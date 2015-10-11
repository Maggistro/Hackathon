#include "Socketmodul.h"


// constructor
Socketmodul::Socketmodul(){
	connectionStatus = false;
	modulePort = SOCKETMODUL_PORT;
	tickRate = MAX_TICK_RATE;
	stopConnection = false;
};


// destructor
Socketmodul::~Socketmodul(){

};

bool Socketmodul::openConnection(){
	return true;
};


bool Socketmodul::closeConnection(){
	stopConnection = true;


	//TODO: wait for connections to be closed


	return true;
};


bool Socketmodul::sendData(PACKAGE package){


	return true;
};


bool Socketmodul::receiveData(PACKAGE package){

	return true;
};


void Socketmodul::changeConnectionTickRate(int newTickRate){
	tickRate = newTickRate;
};


void Socketmodul::printSocketStatus(){
	fprintf(stderr, "Socketstatus");
};

void* Socketmodul::SocketServerTask(void* arg){

};