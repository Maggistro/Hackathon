#include <Queue.h>

Queue& Queue::getInstance(){
	static Queue instance;
	return instance;
}


Queue::~Queue(){

}

void Queue::add(instruction_package instr){
	mtx.lock();
	q.push(instr);
	mtx.unlock();
}

instruction_package Queue::get(){
	
	mtx.lock();
	instruction_package instr;

	instr = (instruction_package)q.front();
	q.pop();
	mtx.unlock();
	return instr;
}
