#ifndef SOCKETMODUL_H
#define SOCKETMODUL_H

// System headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <thread>
#include <iostream>

// its a linux specific library
//#include <pthread.h>
#include <Winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>

#include "DataTypes.h";

// constant declarations
#define SOCKETMODUL_PORT "27015"
#define BUFFER_LENGTH 300

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

class Socketmodul{

public:
	bool isClientConnected;
	unsigned int tickRate;
	bool stopConnection;
	bool connectionStatus;
	SOCKET javaSocket;
	

public:
	Socketmodul();
	~Socketmodul();

	//starts the necessary threads
	bool startServer();

	
	bool openConnection();
	bool closeConnection();	
	char* handlePackage(instruction_package p);

	//some not needed functions :)
	void changeConnectionTickRate(int newTickRate);
	void printSocketStatus();

	//starts the server task
	/*void SocketServerTaskForRead(Socketmodul* socketModul);
	void SocketServerTaskForSend(LPVOID lpParameter);*/
	static DWORD WINAPI SocketServerTaskForSend(LPVOID lpParameter);

};

#endif