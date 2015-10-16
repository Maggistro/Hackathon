#include "Socketmodul.h"


// constructor
Socketmodul::Socketmodul(){
	connectionStatus = false;
	tickRate = MAX_TICK_RATE;
	stopConnection = false;
};


// destructor
Socketmodul::~Socketmodul(){

};

//startServer
bool Socketmodul::startServer(SOCKET *socket){

	std::thread readThread(SocketServerTaskForRead(socket));
	std::thread writeThread(SocketServerTaskForSend(socket));

	std::cout << "both tasks are now started...\n";

	//synchronize threads and wait for ending
	readThread.join();
	writeThread.join();

	return true;
};


// seen in "creating a socket for the server" in WINSOCKET docu.
bool Socketmodul::openConnection(){

	//initialize Winsocket
	WSADATA wsaData;

	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0){
		printf("WSAStartup failed: %d\n", iResult);
		return false;
	}

	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(NULL, SOCKETMODUL_PORT, &hints, &result);

	//that socket tho
	SOCKET listenSocket = INVALID_SOCKET;
	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET){
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	//bind and freeresult
	iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR){
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		return false;
	}
	freeaddrinfo(result);

	//start listen
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR){
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return false;
	}

	//Here is a difference: no "select"-method on the socket

	//the javaSocket is a dummy-client-socket


	javaSocket = INVALID_SOCKET;

	javaSocket = accept(listenSocket, NULL, NULL);
	if (javaSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return false;
	}

	connectionStatus = true;
	return true;
};


bool Socketmodul::closeConnection(){

	int iResult;

	iResult = shutdown(javaSocket, SD_SEND);
	if (iResult == SOCKET_ERROR){
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(javaSocket);
		WSACleanup();
		return false;
	}


	closesocket(javaSocket);
	WSACleanup();
	connectionStatus = false;
	return true;
};


void Socketmodul::printSocketStatus(){
	//TODO: go for it
};


bool Socketmodul::handlePackage(PACKAGE package){

	//TODO: handle packages

	return true;
}


void* Socketmodul::SocketServerTaskForRead(void* arg){

	char buffer[BUFFER_LENGTH];
	int bytesRead = 0;
	bool recvOK, sendOK = true;
	int iResult, iSendResult;

	Socketmodul *pSocketModul = (Socketmodul*)arg;

	pSocketModul->stopConnection = false;

	//start processing loop
	while (!pSocketModul->stopConnection){
		if (pSocketModul->openConnection()){
			while (pSocketModul->isClientConnected && (!pSocketModul->stopConnection)){
				iResult = recv(javaSocket, buffer, BUFFER_LENGTH, 0);
				if (iResult > 0){
					printf("Bytes received: %d\n", iResult);

					//TODO: Check if you get something
				}
				if (!recvOK || !sendOK || pSocketModul->stopConnection){
					pSocketModul->isClientConnected = false;
				}
				if (pSocketModul->stopConnection){
					pSocketModul->closeConnection();
				}
			}
		}
	}
};

void* Socketmodul::SocketServerTaskForSend(void* arg){


	char buffer[BUFFER_LENGTH];
	int bytesRead = 0;
	bool recvOK, sendOK = true;
	int iSendResult;

	Socketmodul *pSocketModul = (Socketmodul*)arg;

	pSocketModul->stopConnection = false;

	//start processing loop
	while (!pSocketModul->stopConnection){
		if (pSocketModul->openConnection()){
			while (pSocketModul->isClientConnected && (!pSocketModul->stopConnection)){
				iSendResult = send(javaSocket, buffer, BUFFER_LENGTH, 0);
				if (iSendResult == SOCKET_ERROR){
					printf("send failed: %d\n", WSAGetLastError());
					closesocket(javaSocket);
					WSACleanup();
					return;
				}
			}
		}
	}
};