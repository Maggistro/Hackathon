#include "CameraServerProgram.h";
#include "Socketmodul.h";
#include "CameraWorker.h";

#include "Queue.h";
#include <thread>

void threadfunction(){

	for (int i = 0; i < 30; i++){
		Queue& q = Queue::getInstance();
		instruction_package pack;
		q.add(pack);
		Sleep(1000);
	}

}

void main(){
	//test
	//create data exchange 
	//thread safe queue containing packages

	//init camera worker 
	CameraWorker* cameraWorker = new CameraWorker();
	std::thread cameraWorkerthread(CameraWorker::processThread, cameraWorker);

	Socketmodul module;
	module.startServer();

	//new producer thread interpreting camera data and creating packages for the queue 

	//init socket server
	//new consumer thread removing packages from the queue and sending them to the robot
	//maybe additional feedback thread waiting for data from robot

	//wait for join/break on/in one thread



}




