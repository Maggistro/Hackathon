#ifndef queue_h
#define queue_h 

// system headers

#include <queue>
#include <mutex>
#include <condition_variable>

#include "datatypes.h";


class Queue{

public:
	static Queue& getInstance();

	void add(instruction_package);
	instruction_package get();
private:	


	Queue(){};
	~Queue();
	std::queue<instruction_package> q;
	std::mutex mtx;
	std::condition_variable cond;
};


#endif