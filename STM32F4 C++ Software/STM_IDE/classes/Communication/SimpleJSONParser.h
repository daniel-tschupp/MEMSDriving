/*
 * SimpleJSONParser.h
 *
 *  Created on: 07.02.2019
 *      Author: tud1
 */

#ifndef COMMUNICATION_SIMPLEJSONPARSER_H_
#define COMMUNICATION_SIMPLEJSONPARSER_H_

extern "C" {
	#include "stm32f4xx_hal.h"
	#include "jsmn.h"
}
#include <memory>
#include <string>
#include <cstring>
#include <sstream>
#include "MEMS_Info.h"
#include "SignalManager.h"

using namespace std;

/**
 * \class SimpleJSONParser
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 02.03.19
 *
 * Description:
 * @n This class provides the possibility to parse a json string and provides the means to search for attributes and their
 * values. At the moment there can only json string with no more than 2 stage be parsed.
 */
class SimpleJSONParser {
	static const uint32_t NUMBEROFTOKENS = 30;		// max number of tokens (tokens are attribute names, values etc...)
	static const uint32_t ATTRIBUTELENGTH = 25;		// max lenght of an attribute
	jsmn_parser mParser;					// the parser object
	jsmntok_t mTokens[NUMBEROFTOKENS];			// an array containing all the tokens (they point to the starting point of an attribute name)
	const char* mpJSString;					// character pointer pointing to the json string.
public:
	uint32_t mLastToken;					//!< Number that indicats how many tokens there are.
	char mActAttribute[ATTRIBUTELENGTH] = {0};		//!< array to temporary store an attribute name.
	char mActValue[ATTRIBUTELENGTH] = {0};			//!< array to temporary store an attribute value.

	/** \brief Constructor that creates a SimpleJSONParser object and parses the given json string.
	 *
	 * @param	jsString	json string to parse
	 */
	SimpleJSONParser(const char* jsString);

	/** \brief Destructor */
	virtual ~SimpleJSONParser();

	/** \brief Stores the parrent name into the temporary attribute name array and returns a pointer to it.
	 *
	 * @return	Pointer to the temporary Attribute name array that contains the parrent name.
	 */
	char* getParrentName();

	/** \brief Searches for the given attribute name inside the parsed json string and stores the corresponding value
	 * inside the temporary attribute value array.
	 *
	 * @return	Pointer to the attribute value array that contains the requested attribute value.
	 */
	char* searchForAttribute(char* pAttributeName);

	/** \brief This method extracts an attribute value/name of a given token number.
	 *
	 * @param	TokenNr		Number of the token that want you want to read.
	 * @param	AttributeArray	Character array to store the value of the token.
	 * @return	Returns the pointer to the AttributeArray.
	 */
	char* getAttribute(unsigned int TokenNr, char* AttributeArray);

	/** \brief This method creates a json string containing all the information of the actual state inside the
	 * Signal Manager object.
	 *
	 * @param	pSigMan		Smart pointer to the signal manager object.
	 * @return	Valid json string containing all the status information.
	 */
	static string crateStatusJSONString(shared_ptr<SignalManager> pSigMan);
};

#endif /* COMMUNICATION_SIMPLEJSONPARSER_H_ */
