/*
 * JSONChecker.cpp
 *
 *  Created on: 19.02.2019
 *      Author: tud1
 */

#include <JSONChecker.h>

/**
 * @n Name: JSONChecker
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This Constructor
 */
JSONChecker::JSONChecker() {}

/**
 * @n Name: ~JSONChecker
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n Destructor
 */
JSONChecker::~JSONChecker() {}

/**
 * @n Name: checkJSONString
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This method checks a parsed json string whether all the needed parameters are inside. Furthermore it checks if
 * all those parameter values are inside the specified ranges.
 */
Errors::ErrorEnum JSONChecker::checkJSONString(SimpleJSONParser& simpleParser, int maxTokens){
	Errors::ErrorEnum err = Errors::NO_ERROR;
	const string cmd = (string)simpleParser.getParrentName();

	if(sCheckerMap.find(cmd) != sCheckerMap.end()){
		vector<JS_Parameter> params = sCheckerMap[cmd];
		if(!params.empty()){
			for(vector<JS_Parameter>::iterator it = params.begin(); it != params.end(); ++it) {
				string attName = it->mName;
				if(it->mAttribute->mType == AttributeType::STRING){
					char tempStr[maxTokens] = {0};
					char* attVal = simpleParser.searchForAttribute((char*)attName.c_str());
					if(*attVal != '\0'){
						sscanf(attVal, "%s", tempStr);
						err = it->mAttribute->isValid(tempStr);
					}
					else{
						err =  Errors::MISSING_PARAMETER;
					}
					if (err != Errors::NO_ERROR)
						break;
				}
				if(it->mAttribute->mType == AttributeType::VALUE){
					float tempVal = -1;
					char* attVal = simpleParser.searchForAttribute((char*)attName.c_str());
					if(*attVal != '\0'){
						sscanf(attVal, "%f", &tempVal);
						err =it->mAttribute->isValid(tempVal);
					}
					else{
						err = Errors::MISSING_PARAMETER;
					}
					if (err != Errors::NO_ERROR)
						break;
				}
			}
		}
	}
	else{
		err = Errors::UNKNOWN_CMD;
	}
	return err;
}

/**
 * @n Name: isJSONString
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n Parses a string to a parsed json string. If it is not possible to parse, it will be indicated by an Error enumeration.
 * It also checks if the whole json message can be parsed.
 */
Errors::ErrorEnum JSONChecker::isJSONString(string jsString, unsigned int maxTokens){
	Errors::ErrorEnum err = Errors::NO_ERROR;

	jsmn_parser parser;
	jsmntok_t tokens[maxTokens];

	const char* js = jsString.c_str();


	jsmn_init(&parser);
	jsmn_parse(&parser, js, strlen(js), tokens, maxTokens);

	if(tokens[0].type !=  JSMN_UNDEFINED){
		unsigned int lastToken = maxTokens - 1;
		for(unsigned int i = 1; i < maxTokens - 1; i++){
			if((tokens[i].end == tokens[0].end-3) || (tokens[i].end == tokens[0].end-2)){ // 3 because of of the end clamps of the json object
				lastToken = (int)i;
				break;
			}
		}
		if(lastToken < 1)
			err = Errors::EMPTY_JSON;
		if(lastToken >= maxTokens)
			err = Errors::TO_MANY_ATTRIBUTES;

	}
	else{
		err = Errors::WRONG_JSON;
	}
	return err;
}
