#include <windows.h> 
#include <stdio.h> 
#include <tchar.h>
#include <strsafe.h>

#define BUFSIZE 512

int mainServer(void)
{
	HANDLE hPipe;
	char buffer[1024];
	DWORD dwRead;


	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Pipe"),
		PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
		PIPE_WAIT,
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				/* add terminating zero */
				buffer[dwRead] = '\0';

				/* do something with data in buffer */
				printf("%s", buffer);
			}
		}

		DisconnectNamedPipe(hPipe);
	}

	return 0;
}

int mainClilent(void)
{
	HANDLE hPipe;
	DWORD dwWritten;


	hPipe = CreateFile(TEXT("\\\\.\\pipe\\Pipe"),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (hPipe != INVALID_HANDLE_VALUE)
	{
		WriteFile(hPipe,
			"Hello Pipe\n",
			12,   // = length of string + terminating '\0' !!!
			&dwWritten,
			NULL);

		CloseHandle(hPipe);
	}

	return (0);
}