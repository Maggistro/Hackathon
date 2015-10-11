#ifndef SOCKETMODUL_H
#define SOCKETMODUL_H

// System headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "DataTypes.h";

// constant declarations
#define SOCKETMODUL_PORT 8080

// send rate or timeout
#define MAX_TICK_RATE  100;


#define MAX_DATA_LENGTH  448
#define DATALENGTH_START  192;
#define DATALENGTH_STOP  0;
#define DATALENGTH_RESET  0;
#define DATALENGTH_THUMB_UP  0;
#define DATALENGTH_THUMB_DOWN  0;
#define DATALENGTH_FOLLOW  192;
#define DATALENGTH_INFO  448;
#define SATALENGTH_STATUS 64;

#define MAX_FAILED_CONNECTION_TRYS  64;

/**
* possible instructions
* "start": motion command for the camera 
* "stop": motion command for the camera
* "reset": resets the robot position and gets it into home-position
* "thumb_up": motion command for the camera
* "thumb_down": motion command for the camera
* "follow": follows the hand
* "info": axis positions
* "status": feedback from the controller/robot
*/
typedef struct{
	char instructionName[64];
	unsigned int dataLength;
}SOCKET_PACKAGE_HEADER;

typedef struct{
	SOCKET_PACKAGE_HEADER dataHeader;
	// TODO: DEFINE DATA 
}SOCKET_PACKAGE;

// instructions that have a body
typedef struct{
	unsigned int x;
	unsigned int y;
	unsigned int z;
}INSTRUCTION_START_BODY;

typedef struct{
	unsigned int x;
	unsigned int y;
	unsigned int z;
}INSTRUCTION_FOLLOW_BODY;

typedef struct{
	int axis_one;
	int axis_two;
	int axis_three;
	int axis_four;
	int axis_five;
	int axis_six;
	int axis_seven;
}INSTRUCTION_INFO_BODY;

typedef struct{
	unsigned int status;
}INSTRUCTION_STATUS_BODY;

class Socketmodul{

private:
	bool connectionStatus;
	unsigned int modulePort;
	unsigned int tickRate;
	bool stopConnection;
	

public:
	Socketmodul();
	~Socketmodul();

	bool openConnection();
	bool closeConnection();	
	bool sendData(PACKAGE package);
	bool receiveData(PACKAGE package);
	void changeConnectionTickRate(int newTickRate);
	void printSocketStatus();

	//starts the server task
	void* SocketServerTask(void* arg);


};

#endif