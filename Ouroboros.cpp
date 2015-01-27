#include "Ouroboros.h"
#include "Arduino.h"

Ouroboros::Ouroboros(){
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
}

void Ouroboros::init(){
	pinMode(charge_pin,OUTPUT);

    pinMode(apa_W_in, INPUT);
    pinMode(apa_W_out,OUTPUT);
	pinMode(apa_N_in, INPUT);
    pinMode(apa_N_out,OUTPUT);
    pinMode(apa_S_in, INPUT);
    pinMode(apa_S_out,OUTPUT);
	
	//wdt_enable(WDTO_2S);

	digitalWrite(charge_pin,HIGH);

    digitalWrite(apa_W_out,LOW);
    digitalWrite(apa_N_out,LOW);
    digitalWrite(apa_S_out,LOW);

    batteryMonitor.reset();
    batteryMonitor.quickStart();
}

void Ouroboros::heartbeat(){
	//wdt_reset();
	the_time = millis();
    // Update system
    if(the_time - up_stat_time > up_stat_period){
        up_stat_time = the_time;
        //Getting on-board battery voltage, and placing it within the packet framework
        batteryMonitor.getVCellBytes(battery_voltage.bytes[0],battery_voltage.bytes[1]);
        battery_voltage.value = batteryMonitor.VCellFromBytes(battery_voltage.bytes[0],battery_voltage.bytes[1]);
   		//
        batteryMonitor.getSoCBytes(battery_soc.bytes[0],battery_soc.bytes[1]);
        battery_soc.value = batteryMonitor.SoCFromBytes(battery_soc.bytes[0],battery_soc.bytes[1]);
        //batt_percent = batteryMonitor.getSoC();
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
}

void Ouroboros::clean_ports(){
	if(port_0.path_in_length==0 and port_0.payload_in_length!=0)
		port_0.payload_in_length = 0;
	if(port_1.path_in_length==0 and port_1.payload_in_length!=0)
		port_1.payload_in_length = 0;
	if(port_2.path_in_length==0 and port_2.payload_in_length!=0)
		port_2.payload_in_length = 0;
	if(port_3.path_in_length==0 and port_3.payload_in_length!=0)
		port_3.payload_in_length = 0;

}

boolean Ouroboros::send_status(struct apa_port_type *port){
	int payload_length = 2;
	boolean success = true;
    switch(port->payload_out[1]){
      	case 'c':{
      	    port->payload_out[0] = 'u';
      	    port->payload_out[1] = 'c';
      	    char charge_stat = '-';
      	    if(charge_status)        
      	        charge_stat = '+';  
      	    port->payload_out[2] = charge_stat;
      	    payload_length++;
      	    break;
      	}
      	case 'v':{
      	    port->payload_out[0] = 'u';
      	    port->payload_out[1] = 'v';
      	    port->payload_out[2] = char(battery_voltage.bytes[0]);
      	    port->payload_out[3] = char(battery_voltage.bytes[1]);
      	    Serial.print(battery_voltage.value,3);
      	    payload_length = 4;
      	    break;          
      	}
      	case 'p':{
      	    port->payload_out[0] = 'u';
      	    port->payload_out[1] = 'p';
      	    port->payload_out[2] = char(battery_soc.bytes[0]);
      	    port->payload_out[3] = char(battery_soc.bytes[1]);
      	    Serial.print(battery_soc.value,2);
      	    payload_length = 4;
      	    break;  
      	}
      	default :
      		success = false;
    }
    port->payload_out_length = payload_length;
    return success;
}

void Ouroboros::toggle_charge_status(struct apa_port_type *port){
    charge_status = !charge_status;
    char charge_stat = '-';
    if(charge_status)        
        charge_stat = '+';
    port->payload_out[0]=charge_stat;
    port->payload_out_length=1;
}



