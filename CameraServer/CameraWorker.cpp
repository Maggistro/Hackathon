#include "CameraWorker.h"


int maximum(_int64 x, _int64 y, _int64 z) {
	int max = abs(x); /* assume x is the largest */
	int coord = 1;
	if (abs(y) > max) { /* if y is larger than max, assign y to max */
		max = abs(y);
		coord = 2;
	    
	} /* end if */

	if (abs(z) > max) { /* if z is larger than max, assign z to max */
		max = abs(z);
		coord = 3;
	} /* end if */

	return coord; /* max is the largest value */
} /* end function maximum */

instruction_package getPackageFollow(Gesture gesture, float offset_handTipX, float offset_handTipY, float offset_handTipZ) {
	instruction_package sp;
	char* temp = "follow";
	memcpy(sp.header, temp, sizeof(temp));

	sp.x = (_int64)(offset_handTipX*1000);
	sp.y = (_int64)(offset_handTipY * 1000);
	sp.z = (_int64)(offset_handTipZ * 1000);
	std::cout << "send follow package with : " << sp.x << " " << sp.y << " " << sp.z << std::endl;
	return sp;

}
instruction_package getPackageStart(Gesture gesture, float offset_handTipX, float offset_handTipY, float offset_handTipZ) {
	instruction_package sp;
	char* temp = "start";
	memcpy(sp.header, temp, sizeof(temp));

	sp.x = (_int64)(offset_handTipX * 1000);
	sp.y = (_int64)(offset_handTipY * 1000);
	sp.z = (_int64)(offset_handTipZ * 1000);
	int result = maximum(sp.x, sp.y, sp.z);
	if (result == 1) {
		sp.y = 0;
		sp.z = 0;
		std::cout << "send start package with : " << sp.x << " " << sp.y << " " << sp.z << std::endl;

	}
	else if (result == 2) {
		sp.x = 0;
		sp.z = 0;
		std::cout << "send start package with : " << sp.x << " " << sp.y << " " << sp.z << std::endl;
	}
	else if (result == 3) {
		sp.x = 0;
		sp.y = 0;
		std::cout << "send start package with : " << sp.x << " " << sp.y << " " << sp.z << std::endl;
		
	}
	else {
		std::cout << "error: no max values" << std::endl;

	}
	return sp;
}
instruction_package getPackageReset(Gesture gesture) {
	instruction_package sp;
	char* temp = "reset";
	memcpy(sp.header, temp, sizeof(temp));
	std::cout << "send reset package" << std::endl;
	return sp;
}



void CameraWorker::configure(CameraWorker* cw)
{
	cw->session = PXCSession::CreateInstance();
	cw->ver = cw->session->QueryVersion();
	wprintf_s(L"SDK Session startet with version %d.%d\n", cw->ver.major, cw->ver.minor);
	cw->senseManager = PXCSenseManager::CreateInstance();
	cw->senseManager->EnableHand();
	cw->senseManager->Init();
	cw->hand = cw->senseManager->QueryHand();
	//handData = hand->CreateOutput();

	cw->config = cw->hand->CreateActiveConfiguration();
	//config->EnableNormalizedJoints(true);
	//config->EnableSegmentationImage(true);
	// Disable initially all gestures
	//config->DisableAllGestures();
	// enable only the needed ones
	cw->config->EnableGesture(L"spreadfingers", true);
	cw->config->EnableGesture(L"two_fingers_pinch_open", true);
	cw->config->EnableGesture(L"wave", true);
	cw->config->EnableAllAlerts();
	cw->config->ApplyChanges();
	//config->Update();
	
}


void CameraWorker::processThread(LPVOID lpParameter)
{
	int counterA = 0;
	int counterB = 0;
	int counterC = 0;
	CameraWorker* cw = (CameraWorker*)lpParameter;

	// Start AcquireFrame/ReleaseFrame loop
	while (cw->senseManager->AcquireFrame(true) >= pxcStatus::PXC_STATUS_NO_ERROR)
	{
		cw->hand = cw->senseManager->QueryHand();

		if (cw->hand != NULL)
		{

			// Retrieve the most recent processed data
			cw->handData = cw->hand->CreateOutput();
			cw->handData->Update();

			// Get number of tracked hands
			cw->nhands = cw->handData->QueryNumberOfHands();

			if (cw->nhands > 0)
			{
				// Retrieve hand identifier
				cw->handData->QueryHandId(PXCHandData::AccessOrderType::ACCESS_ORDER_BY_TIME, 0, cw->handId);

				// Retrieve hand data
				cw->handData->QueryHandDataById(cw->handId, cw->ihand);

				// Retrieve all hand joint data
				for (int i = 0; i < cw->nhands; i++)
				{
					for (int j = 0; j < 0x20; j++)
					{
						PXCHandData::JointData jointData;
						cw->ihand->QueryTrackedJoint((PXCHandData::JointType)j, jointData);
						cw->nodes[i][j] = jointData;
					}
				}

				// Get world coordinates for tip of middle finger on the first hand in camera range
				cw->handTipX = cw->nodes[0][(PXCHandData::JointType::JOINT_MIDDLE_TIP)].positionWorld.x;
				cw->handTipY = cw->nodes[0][(PXCHandData::JointType::JOINT_MIDDLE_TIP)].positionWorld.y;
				cw->handTipZ = cw->nodes[0][(PXCHandData::JointType::JOINT_MIDDLE_TIP)].positionWorld.z;

				if (cw->handTipX == 0 && cw->handTipY == 0 && cw->handTipZ == 0) {
					cw->prev_handTipX = cw->handTipX;
					cw->prev_handTipY = cw->handTipY;
					cw->prev_handTipZ = cw->handTipZ;

					cw->offset_handTipX = cw->handTipX;
					cw->offset_handTipY = cw->handTipY;
					cw->offset_handTipZ = cw->handTipZ;
					//std::cout <<" offset X "<< offset_handTipX <<"offset Y "<< offset_handTipY<< " Z "<<offset_handTipZ<< std::endl;
				}
				else {
					/*if (handData->IsGestureFired(spreadfingers, gestureData)) {
						counterB, counterC = 0;
						counterA++;
						std::cout << "spreadfingers" << std::endl;
						gesture = Gesture::FingerSpread;
						std::cout << counterA << std::endl;
						if (counterA > 30) {
							sendPackageStart(gesture, handTipX, handTipY, handTipZ);
						}
					}
					else*/ 
					if (cw->handData->IsGestureFired(cw->two_fingers_pinch_open, cw->gestureData)) {
						counterA, counterC = 0;
						counterB++;
						std::cout << "two_fingers_pinch_open" << std::endl;
						std::cout << counterB << std::endl;
						cw->gesture = Gesture::Pinch;
						if (counterB > 30) {							
							std::this_thread::sleep_for(std::chrono::milliseconds(200));
							Queue::getInstance().add(getPackageFollow(cw->gesture, cw->offset_handTipX, cw->offset_handTipY, cw->offset_handTipZ));
						}
					}
					else if (cw->handData->IsGestureFired(cw->wave, cw->gestureData)) {
						counterA, counterB = 0;
						counterC++;
						std::cout << "wave" << std::endl; cw->gesture = Gesture::Wave;
						std::cout << counterC << std::endl;
						if (counterC > 30) {
							std::this_thread::sleep_for(std::chrono::milliseconds(200));
							Queue::getInstance().add(getPackageReset(cw->gesture));

						}
					}

					cw->offset_handTipX = cw->handTipX - cw->prev_handTipX;
					cw->offset_handTipY = cw->handTipY - cw->prev_handTipY;
					cw->offset_handTipZ = cw->handTipZ - cw->offset_handTipZ;

					cw->prev_handTipX = cw->handTipX;
					cw->prev_handTipY = cw->handTipY;
					cw->prev_handTipZ = cw->handTipZ;
				}
				// Retrieve gesture data
			}
			else
			{
				cw->gesture = Gesture::Undefined;
			}

			// Get alert status
			for (int i = 0; i < cw->handData->QueryFiredAlertsNumber(); i++)
			{
				PXCHandData::AlertData alertData;
				if (cw->handData->QueryFiredAlertData(i, alertData) != pxcStatus::PXC_STATUS_NO_ERROR) { continue; }

				//Displaying last alert
				switch (alertData.label)
				{
				case PXCHandData::AlertType::ALERT_HAND_DETECTED:
					cw->detectionAlert = "Hand Detected";
					std::cout << "Hand Detected" << std::endl;
					cw->detectionStatusOk = true;
					break;
				case PXCHandData::AlertType::ALERT_HAND_NOT_DETECTED:
					cw->detectionAlert = "Hand Not Detected";
					std::cout << "Hand Not Detected" << std::endl;
					cw->detectionStatusOk = false;
					break;
				case PXCHandData::AlertType::ALERT_HAND_CALIBRATED:
					cw->calibrationAlert = "Hand Calibrated";
					std::cout << "Hand Calibrated" << std::endl;
					cw->calibrationStatusOk = true;
					break;
				case PXCHandData::AlertType::ALERT_HAND_NOT_CALIBRATED:
					cw->calibrationAlert = "Hand Not Calibrated";
					std::cout << "Hand Not Calibrated" << std::endl;
					cw->calibrationStatusOk = false;
					break;
				case PXCHandData::AlertType::ALERT_HAND_INSIDE_BORDERS:
					cw->bordersAlert = "Hand Inside Borders";
					std::cout << "Hand Inside Borders" << std::endl;
					cw->borderStatusOk = true;
					break;
				case PXCHandData::AlertType::ALERT_HAND_OUT_OF_BORDERS:
					cw->bordersAlert = "Hand Out Of Borders";
					std::cout << "Hand Out Of Borders" << std::endl;
					cw->borderStatusOk = false;
					break;
				}
			}

			if (cw->handData != NULL)
				cw->handData->Release();
		}
		cw->senseManager->ReleaseFrame();
	}
}

CameraWorker::CameraWorker()
{
	configure(this);

}
CameraWorker::~CameraWorker()
{
}