#include <Socketmodul.h>
#include <Queue.h>

// constructor
Socketmodul::Socketmodul(){
	connectionStatus = false;
	tickRate = MAX_TICK_RATE;
	stopConnection = false;
	isClientConnected = false;
};


// destructor
Socketmodul::~Socketmodul(){

};

//startServer
bool Socketmodul::startServer(){
	//HANDLE handle;
	std::thread socketThread(Socketmodul::SocketServerTaskForSend,this);
	//if (handle = NULL) std::cout << "handle is null \n";
	//SocketServerTaskForSend(this);
	//closeConnection();
	socketThread.join();

	return true;
};


// seen in "creating a socket for the server" in WINSOCKET docu.
bool Socketmodul::openConnection(){
	std::cout << "in openConnection \n";
	////initialize Winsocket
	WSADATA wsaData;

	int iResult = -2;
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
	//bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
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
	std::cout << "nach accept \n";
	connectionStatus = true;
	isClientConnected = true;
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
	isClientConnected = false;
	return true;
};


void Socketmodul::printSocketStatus(){
	//TODO: go for it
};


char* Socketmodul::handlePackage(instruction_package p){

	char buffer[BUFFER_LENGTH];
	memcpy(buffer, &p, sizeof(instruction_package));
	return buffer;
	

}


void SocketServerTaskForRead(Socketmodul* socket){

	char buffer[BUFFER_LENGTH];
	int bytesRead = 0;
	bool recvOK, sendOK = true;
	int iResult, iSendResult;

	Socketmodul *pSocketModul = (Socketmodul*)socket;

	pSocketModul->stopConnection = false;

	//start processing loop
	while (!pSocketModul->stopConnection){
		if (pSocketModul->openConnection()){
			while (pSocketModul->isClientConnected && (!pSocketModul->stopConnection)){
			//	iResult = recv(javaSocket, buffer, BUFFER_LENGTH, 0);
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



DWORD WINAPI Socketmodul::SocketServerTaskForSend(LPVOID lpParameter){


	char* bufferSend;
	int bytesRead = 0;
	bool recvOK, sendOK = true;
	int iSendResult, iResult;

	Socketmodul *pSocketModul = (Socketmodul*)lpParameter;

	pSocketModul->stopConnection = false;

	

	Queue& q = Queue::getInstance();
	instruction_package package;

	//start processing loop
	while (!pSocketModul->stopConnection){
		if (pSocketModul->openConnection()){
			while (pSocketModul->isClientConnected && (!pSocketModul->stopConnection)){
				//sleeptimer: read 5 times a second
				Sleep(200);
				package = q.get();
				bufferSend = pSocketModul->handlePackage(package);
				std::cout << bufferSend;
				iSendResult = send(pSocketModul->javaSocket, bufferSend, BUFFER_LENGTH, 0);

				if (iSendResult == SOCKET_ERROR){
					printf("send failed: %d\n", WSAGetLastError());
					WSACleanup();
				}
				

			}
		}
	}

	pSocketModul->closeConnection();
	return 0;
};