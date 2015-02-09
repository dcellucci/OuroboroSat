/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////
// Blank_Template 
/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////

//All Ouroboros Sketches should include these libraries:
#include "apa.h"       //Many Thanks to the Center for Bits and Atoms, MIT
#include "MAX17043.h"  //See https://github.com/dcellucci/ArduinoLib_MAX17043 for more details
#include "I2C.h"       //I2C communication library 
#include "Ouroboros.h" //The General Board Spec (v 1.0)

//This is your own addition. For more details, check out
#include "Blank_Ouroboros.h" //Instantiation of Ouroboros we are using here.

Blank_Ouroboros board;

void setup(){
	board.init();
}

void loop(){
	board.heartbeat();
}

void apa_process_packet(struct apa_port_type *port){
	board.process_packet(port);
}