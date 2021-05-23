/*
 * SignalManager.cpp
 *
 *  Created on: 25.02.2019
 *      Author: tud1
 */

#include "SignalManager.h"

/**
 * @n Name: SignalManager
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This constructor method creates a fully usable SignalManager object. 2 Constant 1.65V Signals will be created. The
 * Trigger frequency will also be initialized, as well as the Trigger Bus Frequency.
 */
SignalManager::SignalManager(uint32_t sigUpdateFreq):mSigUpdateFrequency(sigUpdateFreq)
{
	mXVal = 0.0f;
	mYVal = 0.0f;
	mXSigRamp = false;
	mYSigRamp = false;

	this->setNewXSignal(	make_shared<ConstantSignal>(1.0/mSigUpdateFrequency, 0, mXVal, 1.0/mSigUpdateFrequency),
				make_shared<NoFilter>());
	this->setNewYSignal(	make_shared<ConstantSignal>(1.0/mSigUpdateFrequency, 0, mYVal, 1.0/mSigUpdateFrequency),
				make_shared<NoFilter>());

	HAL_DAC_Start(&hdac, DAC1_CHANNEL_2);
	HAL_DAC_Start(&hdac, DAC1_CHANNEL_1);

	mpAScanTriggerPico = make_shared<AScanTrigger>(htim1, TIM_CHANNEL_2, TIM_CHANNEL_3);
	mpAScanTriggerBNC = make_shared<AScanTrigger>(htim3, TIM_CHANNEL_3, TIM_CHANNEL_4);
	uint32_t TriggerFrequency = 10000;
	mpAScanTriggerPico->initTrigerSignal(TriggerFrequency, 84000000);
	mpAScanTriggerPico->stopTriggerSignal();
	mpAScanTriggerBNC->initTrigerSignal(TriggerFrequency, 42000000);
	mpAScanTriggerBNC->stopTriggerSignal();

	miDacSignal = 0;

	mRunningMode = RunningMode::IDLE;
	mScanningMode = ScanMode::SINGLE_SCAN;
	mOCTType = OCTType::SPECTROMETER;
	mErrors = Errors::NO_ERROR;

	mStopCount = mSigUpdateFrequency / (mpSlowAxis)->getFrequency();
	mAScansPerBScan = 1;
	mBScansPerCScan = 1;
	mTrigStopCount = mAScansPerBScan*mBScansPerCScan;

	mXAxisSigType = "ConstSignal";
	mYAxisSigType = "ConstSignal";

}

/**
 * @n Name: setNewXSignal
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This method can be used to change the actual x-channel signal object to a new one without changing the corresponding filter.
 */
void SignalManager::setNewXSignal(std::shared_ptr<Signal> sig){
	mpXSig = sig;
	mpXSig->bind(&mpXFilter);
	this->determineFastAxis();
}

/**
 * @n Name: setNewXSignal
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This method can be used to change the actual x-channel signal object to a new one with adding a new filter too.
 */
void SignalManager::setNewXSignal(std::shared_ptr<Signal> sig, shared_ptr<IFilter> filter){
	mpXFilter = filter;
	this->setNewXSignal(sig);
}

/**
 * @n Name: setNewYSignal
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This method can be used to change the actual y-channel signal object to a new one without changing the corresponding filter.
 */
void SignalManager::setNewYSignal(std::shared_ptr<Signal> sig){
	mpYSig = sig;
	mpYSig->bind(&mpYFilter);
	this->determineFastAxis();
}

/**
 * @n Name: setNewYSignal
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This method can be used to change the actual y-channel signal object to a new one and setting a new filter too.
 */
void SignalManager::setNewYSignal(std::shared_ptr<Signal> sig, shared_ptr<IFilter> filter){
	mpYFilter = filter;
	this->setNewYSignal(sig);
}

/**
 * @n Name: determineFastAxis
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This method compares the two signal frequencies and decides which is the fast and which the slow axis.
 */
void SignalManager::determineFastAxis(void){
	if(mpYSig && mpXSig){
		if(mpYSig->getFrequency() >= mpXSig->getFrequency()){
			mpFastAxis = mpYSig;
			mpSlowAxis = mpXSig;
		}
		else{
			mpFastAxis = mpXSig;
			mpSlowAxis = mpYSig;
		}
	}
}
