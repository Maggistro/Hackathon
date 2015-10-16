#include <windows.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


using namespace std;

class PipeServer{
public:
	static DWORD WINAPI CreatePipeServerThread(LPVOID lpParameter);
	static LPCWSTR GetPipeName();
};
