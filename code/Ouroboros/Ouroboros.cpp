#include "Ouroboros.h"

Ouroboros::Ouroboros(){
  //Port_0 is the debug port, for things like Serial Communication
	port_0.id = '0';
	port_0.path_in_length = 0;
	port_0.path_out_length = 0;
	port_0.next_port = &port_1;
	
	port_1.id = '1';
	port_1.path_in_length = 0;
	port_1.path_out_length = 0;
	port_1.next_port = &port_2;
	port_1.pins_in = portInputRegister(digitalPinToPort(apa_W_in));
  port_1.pin_in = digitalPinToBitMask(apa_W_in);
  port_1.port_out = portOutputRegister(digitalPinToPort(apa_W_out));
  port_1.direction_out = portModeRegister(digitalPinToPort(apa_W_out));
  port_1.pin_out = digitalPinToBitMask(apa_W_out);
	
	port_2.id = '2';
	port_2.path_in_length = 0;
	port_2.path_out_length = 0;
	port_2.next_port = &port_3;
	port_2.pins_in = portInputRegister(digitalPinToPort(apa_N_in));
  port_2.pin_in = digitalPinToBitMask(apa_N_in);
  port_2.port_out = portOutputRegister(digitalPinToPort(apa_N_out));
  port_2.direction_out = portModeRegister(digitalPinToPort(apa_N_out));
  port_2.pin_out = digitalPinToBitMask(apa_N_out);
	
	port_3.id = '3';
	port_3.path_in_length = 0;
	port_3.path_out_length = 0;
	port_3.next_port = &port_0;
	port_3.pins_in = portInputRegister(digitalPinToPort(apa_S_in));
  port_3.pin_in = digitalPinToBitMask(apa_S_in);
  port_3.port_out = portOutputRegister(digitalPinToPort(apa_S_out));
  port_3.direction_out = portModeRegister(digitalPinToPort(apa_S_out));
  port_3.pin_out = digitalPinToBitMask(apa_S_out);

	up_stat_time = 0;
	route_time   = 0;
	debug_time   = 0;

	charge_status = false;
  start_talking = false;
  lipo_gauge_working = true;
}

void Ouroboros::init(){
    //We talk to the LiPo monitor using I2C
    I2c.begin();
    I2c.timeOut(I2C_Timeout);
	  pinMode(charge_pin,OUTPUT);

    pinMode(apa_W_in, INPUT);
    pinMode(apa_W_out,OUTPUT);
	  pinMode(apa_N_in, INPUT);
    pinMode(apa_N_out,OUTPUT);
    pinMode(apa_S_in, INPUT);
    pinMode(apa_S_out,OUTPUT);
	
	  digitalWrite(charge_pin,HIGH);

    digitalWrite(apa_W_out,LOW);
    digitalWrite(apa_N_out,LOW);
    digitalWrite(apa_S_out,LOW);


    error_code = batteryMonitor.reset();
    
    if(error_code == 0)
      error_code = batteryMonitor.quickStart();
    
    if(error_code != 0){
      v_batt.MSB = 0xFE;
      v_batt.LSB = error_code;
      lipo_gauge_working = false;
    }
    else
      lipo_gauge_working = true;

    power_on_delay(); //Healthy delay (as specified by APA)
}

void Ouroboros::heartbeat(){
	the_time = millis();
    // Update system
    if(!lipo_gauge_working && the_time - gauge_update_time > gauge_update_period){
      error_code = batteryMonitor.reset();
      
      if(error_code == 0)
        error_code = batteryMonitor.quickStart();
      
      if(error_code != 0){
        v_batt.MSB = 0xFE;
        v_batt.LSB = error_code;
        lipo_gauge_working = false;
      }
      else
        lipo_gauge_working = true;

    }


    if(the_time - up_stat_time > up_stat_period){
        up_stat_time = the_time;
        //Getting on-board battery voltage, and placing it within the packet framework
        error_code = batteryMonitor.getVCellBytes(v_batt.MSB,v_batt.LSB);
        if(error_code != 0){
          v_batt.MSB = 0xFE;
          v_batt.LSB = error_code;
          lipo_gauge_working = false;
        }
        v_batt.value  = batteryMonitor.VCellFromBytes(v_batt.MSB,v_batt.LSB);
    }

    //if(the_time - route_time > route_period){
    //    route_time = the_time;
    apa_port_scan(&port_0);
    apa_port_scan(&port_1);
    apa_port_scan(&port_2);
    apa_port_scan(&port_3);
    //}

    digitalWrite(charge_pin, !charge_status);
    clean_ports();

    if(the_time > talk_delay && !start_talking)
      start_talking = true;
}

void Ouroboros::clean_ports(){
  //Sometimes, there's junk in the ports.
  //This cleans them up!
	if(port_0.path_in_length==0 and port_0.payload_in_length!=0)
		port_0.payload_in_length = 0;
	if(port_1.path_in_length==0 and port_1.payload_in_length!=0)
		port_1.payload_in_length = 0;
	if(port_2.path_in_length==0 and port_2.payload_in_length!=0)
		port_2.payload_in_length = 0;
	if(port_3.path_in_length==0 and port_3.payload_in_length!=0)
		port_3.payload_in_length = 0;

}

void Ouroboros::clear_port_output(struct apa_port_type *port){
  port->path_out_length = 0;
  port->payload_out_length = 0;
}

boolean Ouroboros::process_packet(struct apa_port_type *port){
  boolean processed = true;
  switch(port->payload_out[0]){
    case 's':{
      send_status(port);
      break;
    }
    case 'c':{
      toggle_charge_status(port);
      break;
    }
    default:{
      processed = false;
    }
  }
  return processed;
}

void Ouroboros::send_status(struct apa_port_type *port){
	int payload_length = 4;
	boolean success = true;
  port->payload_out[0] = 'u';
    switch(port->payload_out[1]){
      	case 'c':{
      	    char charge_stat = '-';
      	    if(charge_status)        
      	        charge_stat = '+';  
      	    port->payload_out[2] = charge_stat;
      	    payload_length = 3;
      	    break;
      	}
      	case 'v':{
      	    port->payload_out[2] = char(v_batt.MSB);
      	    port->payload_out[3] = char(v_batt.LSB);
      	    break;          
      	}
      	case 'p':{
      	    port->payload_out[2] = char(soc_batt.MSB);
      	    port->payload_out[3] = char(soc_batt.LSB);
      	    break;  
      	}
    }
    port->payload_out_length = payload_length;
}

void Ouroboros::send_packet(String path, String payload, struct apa_port_type *port){
  ///*
  port->path_out_length = path.length()+1;
  port->path_out[0]='^';
  for(int i = 0; i < path.length(); i++){
    port->path_out[i+1]=path[i];
  }
  port->payload_out_length = payload.length();
  for(int i = 0; i < payload.length(); i++){
    port->payload_out[i]=payload[i];
  }
  //*/
}

void Ouroboros::toggle_charge_status(struct apa_port_type *port){
    charge_status = !charge_status;
    char charge_stat = '-';
    if(charge_status)        
        charge_stat = '+';
    port->payload_out[0]=charge_stat;
    port->payload_out_length=1;
}




