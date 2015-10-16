#ifndef DATATYPES_H
#define DATATYPES_H 

/**
defines a set of widely used data types for socket and interprocess communication.
*/

typedef struct {
	char* header;
	_int64* data;
}instruction_package;

typedef enum {
	start = 3,// 3 mal integer
	stop = 0,
	reset = 0,
	// optional
	thump_up = 0,
	// optional
	thump_down = 0,
	follow = 3,
	// optional
	info = 7,
	// optional
	status = 1,
}headers;

#endif