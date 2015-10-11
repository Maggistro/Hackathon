#include "CameraServerProgram.h"

void main(){
	//test
	//create data exchange 
	//thread safe queue containing packages

	//init camera worker 
	//new producer thread interpreting camera data and creating packages for the queue 

	//init socket server
	//new consumer thread removing packages from the queue and sending them to the robot
	//maybe additional feedback thread waiting for data from robot

	//wait for join/break on/in one thread
}