/*
 * SimpleJSONParser.cpp
 *
 *  Created on: 07.02.2019
 *      Author: tud1
 */

#include <SimpleJSONParser.h>

/**
 * @n Name: SimpleJSONParser
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 02.03.18
 *
 * Description:
 * @n This constructor parses the given json string (character pointer) with help of the jsmn library:
 * https://gitlab.com/NTPsec/ntpsec/tree/master/libjsmn
 */
SimpleJSONParser::SimpleJSONParser(const char* jsString): mpJSString(jsString) {
	jsmn_init(&mParser);
	jsmn_parse(&mParser, mpJSString, strlen(mpJSString), mTokens, NUMBEROFTOKENS);
	mLastToken = NUMBEROFTOKENS - 1;
	for(unsigned int i = 1; i < NUMBEROFTOKENS - 1; i++){
		if( (mTokens[i].end == mTokens[0].end-3)  || (mTokens[i].end == mTokens[0].end-2) ){
			mLastToken = (uint32_t)i;
			break;
		}
	}
}

/**
 * @n Name: ~SimpleJSONParser
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 02.03.18
 *
 * Description:
 * @n Empty Destructor
 */
SimpleJSONParser::~SimpleJSONParser() {

}

/**
 * @n Name: getParrentName
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 02.03.18
 *
 * Description:
 * @n This method extracts the parrent name out of the json string (usually the second token) and stores it
 * Temporarily inside the mActAttribute array. It returns a pointer to this array.
 */
char* SimpleJSONParser::getParrentName(){
	int i =  mTokens[1].start;
	for( ; i < mTokens[1].end ; i++ )
		mActAttribute[i-mTokens[1].start] = mpJSString[i];
	mActAttribute[i-mTokens[1].start] = '\0';
	return mActAttribute;
}

/**
 * @n Name: searchForAttribute
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 02.03.18
 *
 * Description:
 * @n This method goes through each token and compares it's value to the given attribute name. If it gets a hit, it
 * copies the value of the token afterwards to mActAttribute array and returns a pointer to it.
 */
char* SimpleJSONParser::searchForAttribute(char* pAttributeName){
	unsigned int iTokens = 3;
	for( ; iTokens <= mLastToken; iTokens++){
		char attribute[ATTRIBUTELENGTH] = {0};
		this->getAttribute(iTokens, attribute);
		if(!(strcmp(attribute, pAttributeName))){
			strcpy(mActAttribute, attribute);
			char attValue[ATTRIBUTELENGTH] = {0};
			this->getAttribute(iTokens + 1, attValue);
			strcpy(mActValue, attValue);
			return mActValue;
		}
	}
	mActValue[0] = '\0';
	return mActValue;
}

/**
 * @n Name: getAttribute
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 02.03.18
 *
 * Description:
 * @n This method extracts the value of a token into the given array.
 */
char* SimpleJSONParser::getAttribute(unsigned int TokenNr, char* AttributeArray){
	int iChar = mTokens[TokenNr].start;
	for(  ; iChar < mTokens[TokenNr].end ; iChar++ )
		AttributeArray[iChar-mTokens[TokenNr].start] = mpJSString[iChar];
	AttributeArray[iChar-mTokens[TokenNr].start] = '\0';
	return AttributeArray;
}

/**
 * @n Name: createSTatusJSONString
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 02.03.18
 *
 * Description:
 * @n This method constructs a json string containing the information about the actual state of the signal manager object.
 */
string SimpleJSONParser::crateStatusJSONString(shared_ptr<SignalManager> pSigMan){

	uint32_t temp = pSigMan->mSigUpdateFrequency;
	stringstream jsSStream;
	jsSStream << "{ \"ActState\": { ";
	jsSStream << "\"OCTType\": " << (string)OCTType::octTypeStrVal[pSigMan->mOCTType] << ",";
	jsSStream << "\"DACSamplingFrequency\": " << temp << ",";
	jsSStream << "\"AScanFrequency\": " << pSigMan->mpAScanTriggerPico->getFrequency() << ",";
	jsSStream << "\"ScanningMode\": " << (string)ScanMode::scanModeStrVal[pSigMan->mScanningMode] << ",";
	jsSStream << "\"RunningState\": " << (string)RunningMode::runModeStrVal[pSigMan->mRunningMode] << ",";
	jsSStream << "\"XAxisSignalType\": " << pSigMan->mXAxisSigType << ",";
	jsSStream << "\"YAxisSignalType\": " << pSigMan->mYAxisSigType << ",";
	jsSStream << "\"AScansPerBScan\": " << pSigMan->mAScansPerBScan << ",";
	jsSStream << "\"BScansPerCScan\": " << pSigMan->mBScansPerCScan << ",";
	jsSStream << "\"ErrorState\": " << (string)Errors::errorStrVal[pSigMan->mErrors];
	jsSStream << "}}";

	return jsSStream.str();
}
