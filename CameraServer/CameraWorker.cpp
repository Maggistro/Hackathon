#include "CameraWorker.h"
#include "DataTypes.h"
#include <pxcsession.h>
#include <pxcsensemanager.h>
#include <pxchanddata.h>
#include <pxchandmodule.h>
#include <pxchandconfiguration.h>
#include <iostream>
using namespace std;

class GestureHandler : public PXCSenseManager::Handler

{

public:

	virtual pxcStatus PXCAPI OnNewSample(

		pxcUID, PXCCapture::Sample *sample)

	{
		wprintf_s(L"ENTERED HANDLER\n");
		// return NO ERROR to continue, or 

		// any ERROR to exit the loop

		return PXC_STATUS_NO_ERROR;

	}

};

wchar_t const* fist = L"fist";
wchar_t const* spreadfingers = L"spreadfingers";
wchar_t const* two_fingers_pinch_open = L"two_fingers_pinch_open";
wchar_t const* thumb_up = L"thumb_up";
wchar_t const* thumb_down = L"thumb_down";
wchar_t const* initial = spreadfingers;

PXCHandData::IHand *ihand;
pxcUID handId;
pxcF32 handCenterX;
pxcF32 handCenterY;
pxcF32 handCenterZ;
PXCHandData::JointData nodes[0x20][0x20];

void workWithGesture(wchar_t const* gestureName, PXCHandData *handData){



	if (wcscmp(fist, gestureName) == 0)
	{		
		pxcI32 nhand = handData->QueryNumberOfHands();
	
		//PXCHandData::JointData** nodes;
		//nodes = new PXCHandData::JointData[][] {new PXCHandData::JointData[0x20], new PXCHandData::JointData[0x20]};
		if (nhand > 0)
		{
			// Retrive hand indentifier
			handData->QueryHandData(PXCHandData::AccessOrderType::ACCESS_ORDER_BY_TIME, 0, ihand) == PXC_STATUS_NO_ERROR;
			// retrive hand data
			handData->QueryHandDataById(handId,ihand);

			for (pxcI32 i = 0; i < nhand; i++)
			{
				for (pxcI32 j = 0; j < 0x20; j++)
				{
					PXCHandData::JointData jointData;
					ihand->QueryTrackedJoint((PXCHandData::JointType)j, jointData);
					nodes[i][j] = jointData;
				}

			}

			// get world coordinates
			handCenterX =  nodes[0][PXCHandData::JointType::JOINT_CENTER].positionWorld.x;
			handCenterY = nodes[0][PXCHandData::JointType::JOINT_CENTER].positionWorld.y;
			handCenterZ = nodes[0][PXCHandData::JointType::JOINT_CENTER].positionWorld.z;

			cout <<"X: "<< handCenterX << " Y: " << handCenterX << " Z: " << handCenterZ <<endl;
			
		}
		return;

		}

/*
		if (handData->QueryHandData(PXCHandData::AccessOrderType::ACCESS_ORDER_BY_TIME, handData->QueryNumberOfHands(), ihand) == PXC_STATUS_NO_ERROR)
		{

			pxcI32 c = ihand->QueryTrackingStatus();
			printf("value of c = 0x%08x", c);
		}
*/

		//if (config->SetTrackingMode(PXCHandData::TRACKING_MODE_EXTREMITIES) == true){
		//	for (int j = 0; j < PXCHandData::NUMBER_OF_EXTREMITIES; j++)
		//	{
		//		handData->QueryExtremityPoint((PXCHandData::ExtremityType)j, extremitiesPointsNodes[i][j]);
		//	}
		//}
		//config->SetTrackingMode(PXCHandData::TRACKING_MODE_EXTREMITIES);
		//DisplayJoints(handData);
		//}

		//return;
	
	else if (wcscmp(spreadfingers, gestureName) == 0)
	{
		wprintf_s(L"%s\n", gestureName);
		return;
	}
	else if (wcscmp(two_fingers_pinch_open, gestureName) == 0)
	{
		wprintf_s(L"%s\n", gestureName);
		return;
	}
	else if (wcscmp(thumb_up, gestureName) == 0)
	{
		wprintf_s(L"%s\n", gestureName);
		return;
	}
	else if (wcscmp(thumb_down, gestureName) == 0)
	{
		wprintf_s(L"%s\n", gestureName);
		return;
	}
	return;
};

// Displaying current frames hand joints 
static void DisplayJoints(PXCHandData *handAnalyzer, pxcI64 timeStamp = 0) {

	PXCHandData::JointData nodes[2][PXCHandData::NUMBER_OF_JOINTS] = {};
	PXCHandData::ExtremityData extremitiesPointsNodes[2][PXCHandData::NUMBER_OF_EXTREMITIES] = {};
	//Iterate hands
	for (pxcI32 i = 0; i < handAnalyzer->QueryNumberOfHands(); i++)
	{
		//Get hand by time of appearence
		PXCHandData::IHand* handData;
		if (handAnalyzer->QueryHandData(PXCHandData::AccessOrderType::ACCESS_ORDER_BY_TIME, i, handData) == PXC_STATUS_NO_ERROR)
		{
			pxcF32 r1 = handData->QueryPalmRadiusImage();
			pxcF32 r2 = handData->QueryPalmRadiusWorld();
			pxcI32 c = handData->QueryTrackingStatus();
			PXCHandData::JointData jointData;
			//Iterate Joints
			for (int j = 0; j < PXCHandData::NUMBER_OF_JOINTS; j++)
			{
				handData->QueryNormalizedJoint((PXCHandData::JointType)j, jointData);
				nodes[i][j] = jointData;
			}

			for (int j = 0; j < PXCHandData::NUMBER_OF_EXTREMITIES; j++)
			{
				handData->QueryExtremityPoint((PXCHandData::ExtremityType)j, extremitiesPointsNodes[i][j]);
				pxcF32 size = handData->QueryBodySide();
				handAnalyzer->EXTREMITY_LEFTMOST;
				handAnalyzer->EXTREMITY_RIGHTMOST;
				handAnalyzer->EXTREMITY_CLOSEST;
				handAnalyzer->EXTREMITY_BOTTOMMOST;
				handAnalyzer->EXTREMITY_TOPMOST;

				// TODO: use the size of the object for the forward and backward movement.
				wprintf_s(L"%f\n", size);

			}

		}


	}
};

CameraWorker::CameraWorker()
{
	// initialize my event handler
	GestureHandler hander;

	PXCSession* session = PXCSession::CreateInstance();
	PXCSession::ImplVersion ver = session->QueryVersion();

	wprintf_s(L"SDK Session startet with version %d.%d\n", ver.major, ver.minor);

	PXCSenseManager* sm = PXCSenseManager::CreateInstance();
	sm->EnableHand();

	// stream depth samples WITHOUT handler
	sm->Init();
	// stream depth samples with handler
	//sm->Init(&hander);
	//sm->StreamFrames(true);
	//sm->Close();


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
	
	PXCHandData::ExtremityData extremitiesPointsNodes[2][PXCHandData::NUMBER_OF_EXTREMITIES] = {};

	while (1)
	{
		if (int status = sm->AcquireFrame(true) < PXC_STATUS_NO_ERROR)
		{
			wprintf_s(L"no frame %d\n", status);
			break;
		}

		// Disable initially all gestures
		config->DisableAllGestures();
		// enable only the needed ones
		config->EnableGesture(L"spreadfingers", true);
		config->EnableGesture(L"two_fingers_pinch_open", true);
		config->EnableGesture(L"fist", true);
		config->EnableGesture(L"thumb_up", true);
		config->EnableGesture(L"thumb_down", true);
		config->Update();
		handData->Update();

		for (int id = 0; id < handData->QueryFiredGesturesNumber(); id++)
		{
			if (handData->QueryFiredGestureData(id, gesture) == PXC_STATUS_NO_ERROR)
			{
				workWithGesture(gesture.name, handData);
			}
		}
		config->DisableAllGestures();
		config->Update();
		sm->ReleaseFrame();
	}

	sm->Release();

	session->Release();
}

/*
CameraWorker::CameraWorker()
{
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
*/
CameraWorker::~CameraWorker()
{
}
