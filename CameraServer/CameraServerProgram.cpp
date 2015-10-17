#include "CameraServerProgram.h";
#include "CameraWorker.h";
#include "Socketmodul.h";
#include "Queue.h";


void main(){
	//test
	//create data exchange 
	//thread safe queue containing packages

	//init camera worker 
	CameraWorker cameraWorker;

	Queue& q = Queue::getInstance();
	instruction_package pack;
	pack.header = "klsdjf";
	q.add(pack);

	Socketmodul module;
	module.startServer();

	//new producer thread interpreting camera data and creating packages for the queue 

	//init socket server
	//new consumer thread removing packages from the queue and sending them to the robot
	//maybe additional feedback thread waiting for data from robot

	//wait for join/break on/in one thread



}


