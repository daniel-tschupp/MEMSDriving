/*
 * MEMSScanner.cpp
 *
 *  Created on: 27.11.2018
 *      Author: tud1
 */

#include <MEMSScanner.h>

/**
 * @n Name: MEMS_Scanner
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 29.11.18
 *
 * Description:
 * @n Constructor to create a MEMS_Scanner object. In this constructor all the object used by the MEMS-Software are
 * created.
 *
 * Object List:
 * 	- Hardware (mpHW)
 * 	- SignalManager (mpSigMan)
 * 	- SignalStateMachinev(mpSigStateMachine)
 * 	- JSON Checker (mJSChecker)
 * 	- Signal Factory (mSigFactory)
 * 	- ISR_FIFO_Queue (mQueue)
 */
MEMS_Scanner::MEMS_Scanner(): mQueue(FIFOQueueISRsafe(15))
{
	mpHW = make_shared<Hardware>();
	mpSigMan = make_shared<SignalManager>(20000);
	mpSigStateMachine = make_shared<SigStateMachine>(mpSigMan, mpHW);
}

/**
 * @n Name: ~MEMS_Scanner
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 29.11.18
 *
 * Description:
 * @n Destructor for a MEMS_Scanner object. In this constructor all the object are deleted.
 */
MEMS_Scanner::~MEMS_Scanner() {}

/**
 * @n Name: appExecution
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 29.11.18
 *
 * Description:
 * @n This method is the actual program workflow.
 */
void MEMS_Scanner::appExecution(void){
	mpHW->startDACOutputTimer();

	while (1)
	{
		if(mQueue.getNumberOfElements() > 0){
			switch (mQueue.dequeue()){
			case Tasks::USB_RECEIVE:
				this->taskUSBReceive();
				break;
			case Tasks::WIFI_RECEIVE:
				this->taskWifiReceive();
				break;
			case Tasks::SIG_START:
				mpSigStateMachine->setState(State::START);
				break;
			default:
				HAL_Delay(10);
				break;
			}
		}
		else{
			HAL_Delay(10);
		}
	}
}

/**
 * @n Name: jsonInterpreter
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This method takes the parsed json object and analyses which command it contains to call the correct method to execute it.
 */
void MEMS_Scanner::jsonInterpreter(SimpleJSONParser& jsParser){
	bool NotFound = true;
	char command[20];

	strcpy(command, jsParser.getParrentName());

	if(strcmp(command, "GetState") == 0){
		NotFound = false;
	}
	if(strcmp(command, "RunScan") == 0){
		if(mpSigMan->mScanningMode == ScanMode::NOT_CONFIGURED) {
			mpSigMan->mErrors = Errors::NO_SCANMODE;
			NotFound = false;
		}
		else{
		mpSigMan->mRunningMode = RunningMode::RUN;
		mQueue.enqueue(Tasks::SIG_START);
		NotFound = false;
		}
	}
	if(strcmp(command, "StopScan") == 0){
		mpSigMan->mRunningMode = RunningMode::IDLE;
		NotFound = false;
	}
	if(strcmp(command, "MCUConfig") == 0){
		this->extractMCUConfigInfo(jsParser);
		NotFound = false;
	}
	if(strcmp(command, "SineSignal") == 0){
		mSigFactory.generateSignal("SineSignal", mpSigMan, jsParser);
		NotFound = false;
	}
	if(strcmp(command, "TriangleSignal") == 0){
		mSigFactory.generateSignal("TriangleSignal", mpSigMan, jsParser);
		NotFound = false;
	}
	if(strcmp(command, "ConstantSignal") == 0){
		mSigFactory.generateSignal("ConstantSignal", mpSigMan, jsParser);
		NotFound = false;
	}
	if(strcmp(command, "SetFilter") == 0){
			this->extractFilter(jsParser);
			NotFound = false;
		}
	if(NotFound)
		mpSigMan->mErrors = Errors::UNKNOWN_CMD;
}

/**
 * @n Name: extractMCUConfigInfo
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This method extracts the parameters of a MCU_Config command and stores those information at the corresponding places.
 */
void MEMS_Scanner::extractMCUConfigInfo(SimpleJSONParser& jsParser){
	float aScanFrequency;

	sscanf(jsParser.searchForAttribute((char*)"AScanFrequency"), "%f", &aScanFrequency);

	if(strcmp(jsParser.searchForAttribute((char*)"OCTType"), "SpectrometerOCT") == 0)
		mpSigMan->mOCTType = OCTType::SPECTROMETER;
	if(strcmp(jsParser.searchForAttribute((char*)"OCTType"), "SSOCT") == 0)
		mpSigMan->mOCTType = OCTType::SSOCT;

	if(strcmp(jsParser.searchForAttribute((char*)"ScanningMode"), "SingleScan") == 0)
		mpSigMan->mScanningMode = ScanMode::SINGLE_SCAN;
	if(strcmp(jsParser.searchForAttribute((char*)"ScanningMode"), "ContinuousScan") == 0)
		mpSigMan->mScanningMode = ScanMode::CONTINUOUS_SCAN;

	mpSigMan->mpAScanTriggerPico->initTrigerSignal((uint32_t)aScanFrequency, mpSigMan->mpAScanTriggerPico->mBusCLK);
	mpSigMan->mpAScanTriggerBNC->initTrigerSignal((uint32_t)aScanFrequency, mpSigMan->mpAScanTriggerBNC->mBusCLK);

	mpSigMan->mAScansPerBScan = mpSigMan->mpAScanTriggerPico->getFrequency() / (2.0f*mpSigMan->mpFastAxis->getFrequency());
	mpSigMan->mBScansPerCScan = mpSigMan->mpAScanTriggerPico->getFrequency() / ( 2.0f * mpSigMan->mpSlowAxis->getFrequency() *mpSigMan->mAScansPerBScan);
	if(mpSigMan->mScanningMode == ScanMode::SINGLE_SCAN)
		mpSigMan->mTrigStopCount = mpSigMan->mAScansPerBScan*mpSigMan->mBScansPerCScan;
	else
		mpSigMan->mTrigStopCount = 0;
}

/**
 * @n Name: extractFilter
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 11.03.20
 *
 * Description:
 * @n This method extracts the parameters of a SetFilter command and stores those information at the corresponding places.
 */
void MEMS_Scanner::extractFilter(SimpleJSONParser& jsParser){
	std::vector<float> a;
	std::vector<float> b;

	float Fs;
	float gain;
	float tmp;
	string FilterName(jsParser.searchForAttribute((char*)"FilterName"));
	sscanf(jsParser.searchForAttribute((char*)"Fs"), "%f", &Fs);
	sscanf(jsParser.searchForAttribute((char*)"Gain"), "%f", &gain);

	sscanf(jsParser.searchForAttribute((char*)"a0"), "%f", &tmp);
	a.push_back(tmp);
	sscanf(jsParser.searchForAttribute((char*)"a1"), "%f", &tmp);
	a.push_back(tmp);
	sscanf(jsParser.searchForAttribute((char*)"a2"), "%f", &tmp);
	a.push_back(tmp);
	sscanf(jsParser.searchForAttribute((char*)"a3"), "%f", &tmp);
	a.push_back(tmp);
	sscanf(jsParser.searchForAttribute((char*)"a4"), "%f", &tmp);
	a.push_back(tmp);
	sscanf(jsParser.searchForAttribute((char*)"b0"), "%f", &tmp);
	b.push_back(tmp);
	sscanf(jsParser.searchForAttribute((char*)"b1"), "%f", &tmp);
	b.push_back(tmp);
	sscanf(jsParser.searchForAttribute((char*)"b2"), "%f", &tmp);
	b.push_back(tmp);
	sscanf(jsParser.searchForAttribute((char*)"b3"), "%f", &tmp);
	b.push_back(tmp);
	sscanf(jsParser.searchForAttribute((char*)"b4"), "%f", &tmp);
	b.push_back(tmp);

	mSigFactory.mpFilterFactory->addIIRFilter(FilterName, a, b, gain);
}

/**
 * @n Name: taskUSBReceive
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This method is one of the task methods that are called every 200ms. It checks if new commands have been received
 * via the USB-UART connection. If a complete command was received it checks the validity of the command and excecutes it.
 */
void MEMS_Scanner::taskUSBReceive(void){
	if(mpHW->mpUSB->isDataAvailable())
	{
		char* jsChar = mpHW->mpUSB->readReceiveBuffer();
		if(jsChar[0] != 0){
			mpSigMan->mErrors = Errors::NO_ERROR;
			mpSigMan->mErrors = mJSChecker.isJSONString(jsChar, 25);
			if( mpSigMan->mErrors == Errors::NO_ERROR){
				SimpleJSONParser jsParser(jsChar);
				mpSigMan->mErrors = mJSChecker.checkJSONString(jsParser, 25);
				if(mpSigMan->mErrors == Errors::NO_ERROR)
					this->jsonInterpreter(jsParser);
				}
			std::string jsString = SimpleJSONParser::crateStatusJSONString(mpSigMan);
			mpHW->mpUSB->sendString(jsString);
			mpHW->mpUSB->clearReceiveBuffer();
		}
	}
}

void MEMS_Scanner::taskWifiReceive(void){
	string data = mpHW->mpWifi->checkForData();
	if(data != "")
	{
		mpSigMan->mErrors = Errors::NO_ERROR;
		mpSigMan->mErrors = mJSChecker.isJSONString(data.c_str(), 25);
		if( mpSigMan->mErrors == Errors::NO_ERROR){
			SimpleJSONParser jsParser(data.c_str());
			mpSigMan->mErrors = mJSChecker.checkJSONString(jsParser, 25);
			if(mpSigMan->mErrors == Errors::NO_ERROR){
				this->jsonInterpreter(jsParser);
			}
			std::string jsString = SimpleJSONParser::crateStatusJSONString(mpSigMan);
			mpHW->mpWifi->sendData(jsString);
		}
	}
}
/**
 * @n Name: isrUSBCom
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This method has to be called in the event of a character reception interrupt. It will add the received character to the buffer.
 */
void MEMS_Scanner::isrUSBCom(void){
	if(mpHW->mpUSB != nullptr)
		mpHW->mpUSB->addCharacterToBuffer();
}

void MEMS_Scanner::isrWifiCom(void){
	if(mpHW->mpWifi != nullptr)
		mpHW->mpWifi->addCharacterToBuffer();
}

/**
 * @n Name: regularChecks
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This method is called when the 200ms timer for regular checks throws an interrupt. Here all the regular tasks will be
 * enqueued to the task queue. At the moment the only regular task is to check the USB-UART communication for new commands.
 */
void MEMS_Scanner::regularChecks(void){
	mQueue.enqueue(Tasks::USB_RECEIVE);
	mQueue.enqueue(Tasks::WIFI_RECEIVE);
}

/**
 * @n Name: isrAScanTrigger
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 29.11.18
 *
 * Description:
 * @n This method is called by the AScanTrigger Timer interrupt and executes the task of this interrupt which is the
 * incrementaion of the trigger counter as well as the check if the defined number of triggers already happened.
 */
void MEMS_Scanner::isrAScanTriggerPico(void){
	mpSigMan->mpAScanTriggerPico->CountTrigger();
}
void MEMS_Scanner::isrAScanTriggerBNC(void){
	mpSigMan->mpAScanTriggerBNC->CountTrigger();
}

/**
 * @n Name: isrDAC
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 29.11.18
 *
 * Description:
 * @n This method is called by the DAC Timer interrupt and generates the signal according to the SignalStateMachine
 * state. For more information how its done consult the SignalStateMachine class description.
 */
void MEMS_Scanner::isrDAC(void){
	mpSigStateMachine->executeActualState();
}
