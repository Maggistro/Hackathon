/**
Defines a set of widely used data types for socket and interprocess communication.
*/

typedef struct {
	char* header;
	_int64* data;
} PACKAGE;

typedef enum {
	start = 3,
	stop = 0,
	reset = 0,
	thump_up = 0,
	thump_down = 0,
	follow = 3,
	info = 7,
	status = 1,
} HEADERS;