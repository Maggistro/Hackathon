#include <PipeServer.h>

DWORD WINAPI PipeServer::CreatePipeServerThread(LPVOID lpParameter){
	HANDLE pipe = CreateNamedPipe(GetPipeName(), PIPE_ACCESS_INBOUND | PIPE_ACCESS_OUTBOUND, PIPE_WAIT, 1, 1024, 1024, 120 * 1000, NULL);
	if (pipe == INVALID_HANDLE_VALUE){
		printf("Error at pipe creation: %c\n", GetLastError());
	}
}