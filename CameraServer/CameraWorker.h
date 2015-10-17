#pragma once
#pragma once
//#include "DataTypes.h";
#include <string>
#include "DataTypes.h"
#include <pxcsession.h>
#include <pxcsensemanager.h>
#include <pxchanddata.h>
#include <pxchandmodule.h>
#include <pxchandconfiguration.h>
#include "Queue.h"
#include <Windows.h>
#include <iostream>
#include <thread>
#include <stdio.h>

typedef enum {
	Undefined,
	FingerSpread,
	Pinch,
	Wave
}Gesture;

class CameraWorker
{
public:
	CameraWorker();
	~CameraWorker();



	pxcF32 handCenterX;
	pxcF32 handCenterY;
	pxcF32 handCenterZ;

	pxcF32 XDiff;
	pxcF32 YDiff;
	pxcF32 ZDiff;

	PXCHandConfiguration* config;
	PXCSession* session;
	PXCSession::ImplVersion ver;
	PXCSenseManager* senseManager;
	PXCHandModule *hand;
	PXCHandData *handData;
	PXCHandData::IHand* ihand;
	PXCHandData::GestureData gestureData;
	Gesture gesture;
	pxcI32 nhands;
	pxcI32 handId;
	float handTipX;
	float handTipY;
	float handTipZ;
	float prev_handTipX;
	float prev_handTipY;
	float prev_handTipZ;
	float offset_handTipX;
	float offset_handTipY;
	float offset_handTipZ;
	std::string detectionAlert;
	std::string calibrationAlert;
	std::string bordersAlert;
	bool detectionStatusOk;
	bool calibrationStatusOk;
	bool borderStatusOk;
	PXCHandData::JointData nodes[0x20][0x20];


	//wchar_t const* fist = L"fist";
	wchar_t const* spreadfingers = L"spreadfingers";
	wchar_t const* two_fingers_pinch_open = L"two_fingers_pinch_open";
	//wchar_t const* thumb_up = L"thumb_up";
	//wchar_t const* thumb_down = L"thumb_down";
	wchar_t const* wave = L"wave";
	wchar_t const* initial = spreadfingers;


	//starts camera worker task
	static void processThread(LPVOID lpParameter);
	void configure(CameraWorker* cw);

};
