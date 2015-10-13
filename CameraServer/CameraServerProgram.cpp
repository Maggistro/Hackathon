#include "CameraServerProgram.h"
#include <pxcsession.h>
#include <pxcsensemanager.h>
#include <pxchanddata.h>
#include <pxchandmodule.h>
#include <pxchandconfiguration.h>
#include <iostream>

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

	PXCSession* session = PXCSession::CreateInstance();
	PXCSession::ImplVersion ver = session->QueryVersion();

	wprintf_s(L"SDK Session startet with version %d.%d\n", ver.major, ver.minor);

	PXCSenseManager* sm = PXCSenseManager::CreateInstance();
	sm->EnableHand();

	sm->Init();


	PXCHandModule *hand;
	hand = sm->QueryHand();

	PXCHandData *handData;
	handData = hand->CreateOutput();

	PXCHandConfiguration* config = hand->CreateActiveConfiguration();
	config->EnableNormalizedJoints(true);
	config->EnableSegmentationImage(true);
	config->EnableAllGestures();
	config->ApplyChanges();
	config->Update();

	PXCHandData::GestureData gesture;

	while (1)
	{
		if (int status = sm->AcquireFrame(true) < PXC_STATUS_NO_ERROR)
		{
			wprintf_s(L"no frame %d\n", status);
			break;
		}


		config->DisableAllGestures();
		config->Update();
		handData->Update();

		for (int id = 0; id < handData->QueryFiredGesturesNumber(); id++)
		{
			if (handData->QueryFiredGestureData(id, gesture) == PXC_STATUS_NO_ERROR)
			{
				wprintf_s(L"%s\n", gesture.name);
			}
		}
		config->DisableAllGestures();
		config->Update();
		sm->ReleaseFrame();
	}

	sm->Release();

	session->Release();
}