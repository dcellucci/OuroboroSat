#include "Blank_Ouroboros.h"

Blank_Ouroboros::Blank_Ouroboros(){
	//When we call a constructor, it also calls the constructor for the parent class
}

void Blank_Ouroboros::init(){
	//We can add what we want to the init() method in the Ouroboros class
	Ouroboros::init(); 
	//After we call the original init().
}

void Blank_Ouroboros::heartbeat(){
	//Once again, calling the original heartbeat() method
	Ouroboros::heartbeat();
	//Then below this you can add whatever you want.
}

//You can even write your own methods here.