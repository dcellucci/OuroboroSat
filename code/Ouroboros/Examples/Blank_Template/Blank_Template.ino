//The libraries required to run Ouroboros
//The core functionality
#include "Ouroboros.h"
//Networking
#include "apa.h"
//Battery Status
#include "I2C.h"
#include "MAX17043.h"

//Ouroboros is a class
Ouroboros board;

void setup() {
  //The board needs to be initialized
  board.init();
  // put your setup code here, to run once:
}

void loop() {
  //The board
  board.heartbeat();
  // put your main code here, to run repeatedly:

}

void apa_process_packet(struct apa_port_type *port){
  if(!board.process_packet(port)){
    //you can extend the packet spec to add your own functionality
  }
}
