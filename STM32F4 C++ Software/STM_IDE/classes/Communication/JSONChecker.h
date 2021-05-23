/*
 * JSONChecker.h
 *
 *  Created on: 19.02.2019
 *      Author: tud1
 */

#ifndef COMMUNICATION_JSONCHECKER_H_
#define COMMUNICATION_JSONCHECKER_H_

#include "MEMS_Info.h"
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <limits>
#include <algorithm>
#include <SimpleJSONParser.h>
extern "C"{
	#include "jsmn.h"
}

using namespace std;

/** \brief This enumeration is to define wheter an attribute is a string or a numerical value */
enum class AttributeType {NONE, VALUE, STRING};

/**
 * \class JS_Attribute
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 02.03.19
 *
 * Description:
 * @n This class implements a json Attribute. It defines the isValid method which checks if the attribute in the json
 * string is valid or not.
 */
class JS_Attribute{
public:
	AttributeType mType;		//!< Type of the attribute (numerical, string,...)

	/** \brief Constructor that creates an JS_Attribute object. */
	JS_Attribute(AttributeType type):mType(type){}

	/** \brief Destructor that destroys the JS_Attribute object. */
	~JS_Attribute(){}

	/** \brief Method that checks whether a given numerical value is in the specified range.
	 *
	 * @param	value		Given value.
	 * @return	Error enumeration indicating if the given value is in the specified range.
	 */
	virtual Errors::ErrorEnum isValid(float value){ return Errors::WRONG_PARAM_RANGE; }

	/** \brief Method that checks whether a given numerical value is in the specified range.
	 *
	 * @param	value		Given value.
	 * @return	Error enumeration indicating if the given value is in the specified range.
	 */
	virtual Errors::ErrorEnum isValid(string str){ return Errors::WRONG_PARAM_RANGE; }
};

/**
 * \class ValueAttribute
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 02.03.19
 *
 * Description:
 * @n This class is a specialization for the JS_Attribute class and represents a Value attribute (numerical).
 */
class ValueAttribute : public JS_Attribute{
	float mMinVal, mMaxVal;
public:
	/** \brief Constructor that creates an attribute of the numerical type.
	 *
	 * @param 	minVal		Minimum value the numerical values must have.
	 * @param	maxVal		Maximum value the numerical value must have.
	 */
	ValueAttribute():JS_Attribute(AttributeType::VALUE), mMinVal(std::numeric_limits<float>::min()), mMaxVal(std::numeric_limits<float>::max()){}
	ValueAttribute(float minVal, float maxVal):JS_Attribute(AttributeType::VALUE), mMinVal(minVal), mMaxVal(maxVal){}

	/** \brief Destructor of the ValueAttribute object. */
	~ValueAttribute(){}

	/** \brief This method checks whether the given value is inside the defined range.
	 *
	 * @param	value		The value to be checked.
	 * @return	Error enumeration indicating if the given value is inside the specified range.
	 */
	Errors::ErrorEnum isValid(float value){
		if( value >= mMinVal && value <= mMaxVal )
			return Errors::NO_ERROR;
		else
			return Errors::WRONG_PARAM_RANGE;
	}
};

/**
 * \class StringAttribute
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 02.03.19
 *
 * Description:
 * @n This class is a specialization for the JS_Attribute class and represents a String attribute.
 */
class StringAttribute : public JS_Attribute{
	vector<string> mStrList;
public:
	/** \brief Constructor that creates an attribute of the type string.
	 *
	 * @param	strList		List of valid strings.
	 */
	StringAttribute(vector<string> strList): JS_Attribute(AttributeType::STRING), mStrList(strList){}

	/** \brief Destructor of StringAttribute object. */
	~StringAttribute(){}

	/** \brief Method to check whether the attribute is one of the allowed strings.
	 *
	 * @param	str	string to be checked.
	 * @return	Error enumeration indication the validity of the given string.
	 */
	Errors::ErrorEnum isValid(string str){
		if(mStrList.empty())
			return Errors::NO_ERROR;
		if( find(mStrList.begin(), mStrList.end(), str) != mStrList.end() )
			return Errors::NO_ERROR;
		else
			return Errors::WRONG_PARAM_RANGE;
	}
};

/**
 * \class JS_Parameter
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 02.03.19
 *
 * Description:
 * @n This class represents a json Parameter. The Parameter contains an attribute name and an attribute.
 */
class JS_Parameter{
public:
	string mName;			//!< Name of the Attribute.
	JS_Attribute* mAttribute;	//!< Value of the Attribute.

	/** \brief Constructor that initializes a JS_Parameter object with a Name and a Value
	 *
	 * @param	name		Name of the Attribute.
	 * @param	attribute	Value of the Attribute.
	 */
	JS_Parameter(string name, JS_Attribute* attribute): mName(name), mAttribute(attribute){}

	/** \brief Destructor og JS_Parameter object. */
	~JS_Parameter(){}
};

/**
 * \class JSONChecker
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 02.03.19
 *
 * Description:
 * @n The JSONChecker class contains all the information to check wheter a json string is correct, contains the right Parameter
 * and furthermore it checks if all the attribute are in the defined ranges. This class has to be modified if one want to
 * add a new command or new parameters to an existing command.
 */
class JSONChecker {
public:
	/** \brief Map connecting cmd_string to JS_Parameter. */
	typedef map<std::string, vector<JS_Parameter>> CheckMap;

	/** \brief Parameter list for MCU_Config command. */
	vector<JS_Parameter> sMCUParamList = {
			JS_Parameter("OCTType", new StringAttribute({ "SpectrometerOCT", "SSOCT"})),
			JS_Parameter("AScanFrequency", new ValueAttribute(1.0f, 120000.0f)),
			JS_Parameter("ScanningMode", new StringAttribute({ "SingleScan", "ContinuousScan"}))
	};

	/** \brief Parameter list for SineSignal command. */
	vector<JS_Parameter> sSineSignalParamList = {
			JS_Parameter("OutputStage", new StringAttribute({ "XAxis", "YAxis"})),
			JS_Parameter("Frequency", new ValueAttribute(0.001f, 1500.0f)),
			JS_Parameter("Amplitude", new ValueAttribute(0.0f, 1.5f)),
			JS_Parameter("Phase", new ValueAttribute(0.0f, 360.0f)),
			JS_Parameter("Offset", new ValueAttribute(-1.65f, 1.65f)),
			JS_Parameter("Filter", new StringAttribute({ }))
	};

	/** \brief Parameter list for TriangleSignal command. */
	vector<JS_Parameter> sTriangleSignalParamList = {
			JS_Parameter("OutputStage", new StringAttribute({ "XAxis", "YAxis"})),
			JS_Parameter("Frequency", new ValueAttribute(0.001f, 1500.0f)),
			JS_Parameter("Amplitude", new ValueAttribute(0.0f, 1.5f)),
			JS_Parameter("Phase", new ValueAttribute(0.0f, 360.0f)),
			JS_Parameter("Offset", new ValueAttribute(-1.5f, 1.5f)),
			JS_Parameter("Width", new ValueAttribute(0.0f, 1.0f)),
			JS_Parameter("Filter", new StringAttribute({ }))
	};

	/** \brief Parameter list for ConstantSignal command. */
	vector<JS_Parameter> sConstantSignalParamList= {
			JS_Parameter("OutputStage", new StringAttribute({ "XAxis", "YAxis"})),
			JS_Parameter("Duration", new ValueAttribute(0.000001f, 15000.0f)),
			JS_Parameter("DCValue", new ValueAttribute(-1.5f, 1.5f)),
			JS_Parameter("Offset", new ValueAttribute(-1.5f, 1.5f)),
			JS_Parameter("Filter", new StringAttribute({ }))
	};

	/** \brief Parameter list for Filter Creation command. */
	vector<JS_Parameter> sFilterParamList= {
				JS_Parameter("FilterName", new StringAttribute({})),
				JS_Parameter("Fs", new ValueAttribute({ 1, 100000})),
				JS_Parameter("Gain", new ValueAttribute({ -10000.0f, 10000.0f})),
				JS_Parameter("a0", new ValueAttribute(-10000.0f, 10000.0f)),
				JS_Parameter("a1", new ValueAttribute(-10000.0f, 10000.0f)),
				JS_Parameter("a2", new ValueAttribute(-10000.0f, 10000.0f)),
				JS_Parameter("a3", new ValueAttribute(-10000.0f, 10000.0f)),
				JS_Parameter("a4", new ValueAttribute(-10000.0f, 10000.0f)),
				JS_Parameter("b0", new ValueAttribute(-10000.0f, 10000.0f)),
				JS_Parameter("b1", new ValueAttribute(-10000.0f, 10000.0f)),
				JS_Parameter("b2", new ValueAttribute(-10000.0f, 10000.0f)),
				JS_Parameter("b3", new ValueAttribute(-10000.0f, 10000.0f)),
				JS_Parameter("b4", new ValueAttribute(-10000.0f, 10000.0f))
		};

	/** \brief Map containing all the valid commands with all their parameters. */
	CheckMap sCheckerMap = {	{"MCUConfig", JSONChecker::sMCUParamList},
					{"SineSignal", JSONChecker::sSineSignalParamList},
					{"TriangleSignal", JSONChecker::sTriangleSignalParamList},
					{"ConstantSignal", JSONChecker::sConstantSignalParamList},
					{"GetState", vector<JS_Parameter>()},
					{"RunScan", vector<JS_Parameter>()},
					{"StopScan", vector<JS_Parameter>()},
					{"SetFilter", JSONChecker::sFilterParamList}
	};

	/** \brief Constructor for JSONChecker object */
	JSONChecker();

	/** \brief Destructor for JSONChecker object */
	virtual ~JSONChecker();

	/** \brief This method checks a parsed json string if the command is known, it contains all the needed parameter
	 * and those parameters are in the correct range.
	 *
	 * @param	simpleParser		Parsed json string
	 * @param	maxTokens		Number of maximal tokens inside the simpleParser
	 * @return	Returns the Error enumeration according to the evaluation of this checker method.
	 */
	Errors::ErrorEnum checkJSONString(SimpleJSONParser& simpleParser, int maxTokens);

	/** \brief This method checks if a given string is an json string.
	 *
	 * @param 	jsString		String to be checked.
	 * @param	maxTokens		Maximal number of tokens that shall be parsed.
	 */
	Errors::ErrorEnum isJSONString(string jsString, unsigned int maxTokens);
};

#endif /* COMMUNICATION_JSONCHECKER_H_ */

