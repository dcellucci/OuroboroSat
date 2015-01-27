#include "apa.h"
#include "MAX17043.h" //See https://github.com/dcellucci/ArduinoLib_MAX17043 for more details
//#include <avr/wdt.h>

#ifndef _OUROBOROS_H
#define _OUROBOROS_H


//Pin definitions
#define charge_pin 10

//length of update time, in milliseconds
#define up_stat_period 1000 //update the variables containing the
#define route_period   1   //route the packets
#define debug_period   1000

#define apa_N_in A0
#define apa_N_out A1
#define apa_S_in A2
#define apa_S_out A3
#define apa_W_in 2
#define apa_W_out 3

//Data Packet Specification:
union packet{
    byte bytes[2];
    float value;
};

class Ouroboros{

	public:

		Ouroboros();

		void init();
		void heartbeat();
		void toggle_charge_status(struct apa_port_type *port);
		boolean send_status(struct apa_port_type *port);
		//
		//accessors
		//
		MAX17043 getBattMonitor();

	protected:
		packet battery_voltage, battery_soc;
		//Each Ouroboros Board has four virtual APA ports
		struct apa_port_type port_0, port_1, port_2, port_3;
		// as well as an IC for sensing the battery Voltage/Current
		MAX17043 batteryMonitor; //Battery Monitor object
		//Then there are a bunch of variables that are necessary for the running of the machine.
		//Timer variables
		long the_time;
		//temp variables (necessary for this non-delay architecture)
		//See the "Blink without Delay" sketch in Arduino's examples for
		//A good introduction to this functionality.
		long up_stat_time;
		long route_time;
		long debug_time;


		float batt_voltage, batt_percent;

		//sets the MOSFET that does the charging
		boolean charge_status;
	private:
		void clean_ports();

};

#endif