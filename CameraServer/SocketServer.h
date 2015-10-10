#pragma once
#include "DataTypes.h";
#include <string>

class SocketServer
{
public:
	//inits socket server object
	SocketServer(std::string address, int port);
	//standart destructor
	~SocketServer();
	//send a package over the port
	int send(PACKAGE package);
	//receive a package on the port
	int receive(PACKAGE package);

	//starts the server task
	void* SocketServerTask(void* arg);
};

