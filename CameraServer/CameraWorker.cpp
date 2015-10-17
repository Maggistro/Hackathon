#include "CameraWorker.h"
#include "DataTypes.h"
#include <pxcsession.h>
#include <pxcsensemanager.h>
#include <pxchanddata.h>
#include <pxchandmodule.h>
#include <pxchandconfiguration.h>
#include <iostream>
#include <thread>

using namespace std;
//wchar_t const* fist = L"fist";
wchar_t const* spreadfingers = L"spreadfingers";
wchar_t const* two_fingers_pinch_open = L"two_fingers_pinch_open";
//wchar_t const* thumb_up = L"thumb_up";
//wchar_t const* thumb_down = L"thumb_down";
wchar_t const* wave = L"wave";
wchar_t const* initial = spreadfingers;

typedef enum {
	Undefined,
	FingerSpread,
	Pinch,
	Wave
}Gesture;

instruction_package sendPackageFollow(Gesture gesture, float offset_handTipX, float offset_handTipY, float offset_handTipZ) {
	instruction_package sp;
	sp.header = "follow";
	sp.offsetX = offset_handTipX;
	sp.offsetY = offset_handTipY;
	sp.offsetZ = offset_handTipZ;
	cout << "send follow package with : " << sp.offsetX << " " << sp.offsetY << " " << sp.offsetZ << endl;
	return sp;

}
instruction_package sendPackageStart(Gesture gesture, float handTipX, float handTipY, float handTipZ) {
	instruction_package sp;
	sp.header = "start";
	sp.x = handTipX;
	sp.y = handTipY;
	sp.z = handTipZ;
	cout << "send start package with : " << sp.x << " " << sp.y << " " << sp.z << endl;
	return sp;
}
instruction_package sendPackageReset(Gesture gesture) {
	instruction_package sp;
	sp.header = "reset";
	cout << "send reset package" << endl;
	return sp;
}


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
string detectionAlert;
string calibrationAlert;
string bordersAlert;
bool detectionStatusOk;
bool calibrationStatusOk;
bool borderStatusOk;
PXCHandData::JointData nodes[0x20][0x20];


void configure()
{
	session = PXCSession::CreateInstance();
	ver = session->QueryVersion();
	wprintf_s(L"SDK Session startet with version %d.%d\n", ver.major, ver.minor);
	senseManager = PXCSenseManager::CreateInstance();
	senseManager->EnableHand();
	senseManager->Init();
	hand = senseManager->QueryHand();
	//handData = hand->CreateOutput();

	config = hand->CreateActiveConfiguration();
	//config->EnableNormalizedJoints(true);
	//config->EnableSegmentationImage(true);
	// Disable initially all gestures
	//config->DisableAllGestures();
	// enable only the needed ones
	config->EnableGesture(L"spreadfingers", true);
	config->EnableGesture(L"two_fingers_pinch_open", true);
	config->EnableGesture(L"wave", true);
	config->EnableAllAlerts();
	config->ApplyChanges();
	//config->Update();

}


void processThread()
{
	int counterA = 0;
	int counterB = 0;
	int counterC = 0;
	// Start AcquireFrame/ReleaseFrame loop
	while (senseManager->AcquireFrame(true) >= pxcStatus::PXC_STATUS_NO_ERROR)
	{
		hand = senseManager->QueryHand();

		if (hand != NULL)
		{

			// Retrieve the most recent processed data
			handData = hand->CreateOutput();
			handData->Update();

			// Get number of tracked hands
			nhands = handData->QueryNumberOfHands();

			if (nhands > 0)
			{
				// Retrieve hand identifier
				handData->QueryHandId(PXCHandData::AccessOrderType::ACCESS_ORDER_BY_TIME, 0, handId);

				// Retrieve hand data
				handData->QueryHandDataById(handId, ihand);

				// Retrieve all hand joint data
				for (int i = 0; i < nhands; i++)
				{
					for (int j = 0; j < 0x20; j++)
					{
						PXCHandData::JointData jointData;
						ihand->QueryTrackedJoint((PXCHandData::JointType)j, jointData);
						nodes[i][j] = jointData;
					}
				}

				// Get world coordinates for tip of middle finger on the first hand in camera range
				handTipX = nodes[0][(PXCHandData::JointType::JOINT_MIDDLE_TIP)].positionWorld.x;
				handTipY = nodes[0][(PXCHandData::JointType::JOINT_MIDDLE_TIP)].positionWorld.y;
				handTipZ = nodes[0][(PXCHandData::JointType::JOINT_MIDDLE_TIP)].positionWorld.z;

				if (handTipX == 0 && handTipY == 0 && handTipZ == 0) {
					prev_handTipX = handTipX;
					prev_handTipY = handTipY;
					prev_handTipZ = handTipZ;

					offset_handTipX = handTipX;
					offset_handTipY = handTipY;
					offset_handTipZ = handTipZ;
					//cout <<" offset X "<< offset_handTipX <<"offset Y "<< offset_handTipY<< " Z "<<offset_handTipZ<< endl;
				}
				else {
					if (handData->IsGestureFired(spreadfingers, gestureData)) {
						counterB, counterC = 0;
						counterA++;
						cout << "spreadfingers" << endl;
						gesture = Gesture::FingerSpread;
						cout << counterA << endl;
						if (counterA > 30) {
							sendPackageStart(gesture, handTipX, handTipY, handTipZ);
						}
					}
					else if (handData->IsGestureFired(two_fingers_pinch_open, gestureData)) {
						counterA, counterC = 0;
						counterB++;
						cout << "two_fingers_pinch_open" << endl;
						cout << counterB << endl;
						gesture = Gesture::Pinch;
						if (counterB > 30) {
							sendPackageFollow(gesture, offset_handTipX, offset_handTipY, offset_handTipZ);
						}
					}
					else if (handData->IsGestureFired(wave, gestureData)) {
						counterA, counterB = 0;
						counterC++;
						cout << "wave" << endl; gesture = Gesture::Wave;
						cout << counterC << endl;
						if (counterC > 30) {
							sendPackageReset(gesture);
						}
					}

					offset_handTipX = handTipX - prev_handTipX;
					offset_handTipY = handTipY - prev_handTipY;
					offset_handTipZ = handTipZ - offset_handTipZ;

					prev_handTipX = handTipX;
					prev_handTipY = handTipY;
					prev_handTipZ = handTipZ;
				}
				// Retrieve gesture data
			}
			else
			{
				gesture = Gesture::Undefined;
			}

			// Get alert status
			for (int i = 0; i < handData->QueryFiredAlertsNumber(); i++)
			{
				PXCHandData::AlertData alertData;
				if (handData->QueryFiredAlertData(i, alertData) != pxcStatus::PXC_STATUS_NO_ERROR) { continue; }

				//Displaying last alert
				switch (alertData.label)
				{
				case PXCHandData::AlertType::ALERT_HAND_DETECTED:
					detectionAlert = "Hand Detected";
					cout << "Hand Detected" << endl;
					detectionStatusOk = true;
					break;
				case PXCHandData::AlertType::ALERT_HAND_NOT_DETECTED:
					detectionAlert = "Hand Not Detected";
					cout << "Hand Not Detected" << endl;
					detectionStatusOk = false;
					break;
				case PXCHandData::AlertType::ALERT_HAND_CALIBRATED:
					calibrationAlert = "Hand Calibrated";
					cout << "Hand Calibrated" << endl;
					calibrationStatusOk = true;
					break;
				case PXCHandData::AlertType::ALERT_HAND_NOT_CALIBRATED:
					calibrationAlert = "Hand Not Calibrated";
					cout << "Hand Not Calibrated" << endl;
					calibrationStatusOk = false;
					break;
				case PXCHandData::AlertType::ALERT_HAND_INSIDE_BORDERS:
					bordersAlert = "Hand Inside Borders";
					cout << "Hand Inside Borders" << endl;
					borderStatusOk = true;
					break;
				case PXCHandData::AlertType::ALERT_HAND_OUT_OF_BORDERS:
					bordersAlert = "Hand Out Of Borders";
					cout << "Hand Out Of Borders" << endl;
					borderStatusOk = false;
					break;
				}
			}

			if (handData != NULL)
				handData->Release();
		}
		senseManager->ReleaseFrame();
	}
}

CameraWorker::CameraWorker()
{
	configure();
	processThread();

}
CameraWorker::~CameraWorker()
{
}