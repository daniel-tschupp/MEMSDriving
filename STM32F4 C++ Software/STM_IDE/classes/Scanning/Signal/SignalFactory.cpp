/*
 * SignalFactory.cpp
 *
 *  Created on: 26.02.2019
 *      Author: tud1
 */

#include <SignalFactory.h>

SignalFactory::SignalFactory() {
	mpFilterFactory = std::make_shared<FilterFactory>();
	mSigFacMap["SineSignal"] = make_shared<SineSignalFactory>(mpFilterFactory);
	mSigFacMap["TriangleSignal"] = make_shared<TriangleSignalFactory>(mpFilterFactory);
	mSigFacMap["ConstantSignal"] = make_shared<ConstantSignalFactory>(mpFilterFactory);
}

SignalFactory::~SignalFactory() {
}
void SignalFactory::generateSignal(string sigName, shared_ptr<SignalManager>& sigMan, SimpleJSONParser& jsParser){
	mSigFacMap[sigName]->generateSignal(sigMan, jsParser);
}

SineSignalFactory::SineSignalFactory(std::shared_ptr<FilterFactory> filterFactory){
	mpFilterFactory = filterFactory;
}
void SineSignalFactory::generateSignal(shared_ptr<SignalManager>& sigMan, SimpleJSONParser& jsParser){
	char outStage[25];
	char filter[25];
	float freq, amp, phase, offset;

	sscanf(jsParser.searchForAttribute((char*)"OutputStage"), "%s", outStage);
	sscanf(jsParser.searchForAttribute((char*)"Frequency"), "%f", &freq);
	sscanf(jsParser.searchForAttribute((char*)"Amplitude"), "%f", &amp);
	sscanf(jsParser.searchForAttribute((char*)"Phase"), "%f", &phase);
	sscanf(jsParser.searchForAttribute((char*)"Offset"), "%f", &offset);
	sscanf(jsParser.searchForAttribute((char*)"Filter"), "%s", filter);

	if(strcmp(outStage, "XAxis") == 0){
		sigMan->setNewXSignal(
			make_shared<SinSignal>(freq, phase, amp, offset, 1.0/sigMan->mSigUpdateFrequency),
			mpFilterFactory->generateFilter((string)filter));
		sigMan->mXAxisSigType = "SineSignal";
	}
	if(strcmp(outStage, "YAxis") == 0){
		sigMan->setNewYSignal(
				make_shared<SinSignal>(freq, phase, amp, offset, 1.0/sigMan->mSigUpdateFrequency),
				mpFilterFactory->generateFilter((string)filter));
		sigMan->mYAxisSigType = "SineSignal";
	}

	sigMan->mStopCount = sigMan->mSigUpdateFrequency / sigMan->mpSlowAxis->getFrequency();
	sigMan->mAScansPerBScan = sigMan->mpAScanTriggerPico->getFrequency() / (2.0f*sigMan->mpFastAxis->getFrequency());
	sigMan->mBScansPerCScan = sigMan->mpAScanTriggerPico->getFrequency() / ( 2.0f * sigMan->mpSlowAxis->getFrequency() * sigMan->mAScansPerBScan);
	if(sigMan->mScanningMode == ScanMode::SINGLE_SCAN)
		sigMan->mTrigStopCount = sigMan->mAScansPerBScan*sigMan->mBScansPerCScan;
	else
		sigMan->mTrigStopCount = 0;
}

TriangleSignalFactory::TriangleSignalFactory(std::shared_ptr<FilterFactory> filterFactory){
	mpFilterFactory = filterFactory;
}
void TriangleSignalFactory::generateSignal(shared_ptr<SignalManager>& sigMan, SimpleJSONParser& jsParser){
	char outStage[25];
	char filter[25];
	float freq, amp, phase, offset, width;

	sscanf(jsParser.searchForAttribute((char*)"OutputStage"), "%s", outStage);
	sscanf(jsParser.searchForAttribute((char*)"Frequency"), "%f", &freq);
	sscanf(jsParser.searchForAttribute((char*)"Amplitude"), "%f", &amp);
	sscanf(jsParser.searchForAttribute((char*)"Phase"), "%f", &phase);
	sscanf(jsParser.searchForAttribute((char*)"Offset"), "%f", &offset);
	sscanf(jsParser.searchForAttribute((char*)"Width"), "%f", &width);
	sscanf(jsParser.searchForAttribute((char*)"Filter"), "%s", filter);

	if(strcmp(outStage, "XAxis") == 0){
		sigMan->setNewXSignal(
				make_shared<TriangleSignal>(sigMan->mSigUpdateFrequency, freq, amp, phase, offset, width),
				mpFilterFactory->generateFilter((string)filter));
		sigMan->mXAxisSigType = "TriangleSignal";
	}
	if(strcmp(outStage, "YAxis") == 0){
		sigMan->setNewYSignal(
				make_shared<TriangleSignal>(sigMan->mSigUpdateFrequency, freq, amp, phase, offset, width),
				mpFilterFactory->generateFilter((string)filter));
		sigMan->mYAxisSigType = "TriangleSignal";
	}

	sigMan->mStopCount = sigMan->mSigUpdateFrequency / sigMan->mpSlowAxis->getFrequency();
	sigMan->mAScansPerBScan = sigMan->mpAScanTriggerPico->getFrequency() / (2.0f*sigMan->mpFastAxis->getFrequency());
	sigMan->mBScansPerCScan = sigMan->mpAScanTriggerPico->getFrequency() / ( 2.0f * sigMan->mpSlowAxis->getFrequency() * sigMan->mAScansPerBScan);
	if(sigMan->mScanningMode == ScanMode::SINGLE_SCAN)
		sigMan->mTrigStopCount = sigMan->mAScansPerBScan*sigMan->mBScansPerCScan;
	else
		sigMan->mTrigStopCount = 0;
}

ConstantSignalFactory::ConstantSignalFactory(std::shared_ptr<FilterFactory> filterFactory){
	mpFilterFactory = filterFactory;
}
void ConstantSignalFactory::generateSignal(shared_ptr<SignalManager>& sigMan, SimpleJSONParser& jsParser){
	char outStage[25];
	char filter[25];
	float duration, dcVal, offset;

	sscanf(jsParser.searchForAttribute((char*)"OutputStage"), "%s", outStage);
	sscanf(jsParser.searchForAttribute((char*)"Duration"), "%f", &duration);
	sscanf(jsParser.searchForAttribute((char*)"DCValue"), "%f", &dcVal);
	sscanf(jsParser.searchForAttribute((char*)"Offset"), "%f", &offset);
	sscanf(jsParser.searchForAttribute((char*)"Filter"), "%s", filter);

	if(strcmp(outStage, "XAxis") == 0){
		sigMan->setNewXSignal(
			make_shared<ConstantSignal>(duration, dcVal, offset, 1.0/sigMan->mSigUpdateFrequency),
			mpFilterFactory->generateFilter((string)filter));
		sigMan->mXAxisSigType = "ConstantSignal";
	}
	if(strcmp(outStage, "YAxis") == 0){
		sigMan->setNewYSignal(
			make_shared<ConstantSignal>(duration, dcVal, offset, 1.0/sigMan->mSigUpdateFrequency),
			mpFilterFactory->generateFilter((string)filter));
		sigMan->mYAxisSigType = "ConstantSignal";
	}

	sigMan->mStopCount = sigMan->mSigUpdateFrequency / sigMan->mpSlowAxis->getFrequency();
	sigMan->mAScansPerBScan = sigMan->mpAScanTriggerPico->getFrequency() / (2.0f*sigMan->mpFastAxis->getFrequency());
	sigMan->mBScansPerCScan = sigMan->mpAScanTriggerPico->getFrequency() / ( 2.0f * sigMan->mpSlowAxis->getFrequency() * sigMan->mAScansPerBScan);
	if(sigMan->mScanningMode == ScanMode::SINGLE_SCAN)
		sigMan->mTrigStopCount = sigMan->mAScansPerBScan*sigMan->mBScansPerCScan;
	else
		sigMan->mTrigStopCount = 0;
}
