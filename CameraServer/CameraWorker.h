#pragma once
#pragma once
//#include "DataTypes.h";

class CameraWorker
{
public:
	CameraWorker();
	~CameraWorker();


	//starts camera worker task
	void* CameraWorkerTask(void* arg);



};
