#ifndef MEMS_INFO_H_
#define MEMS_INFO_H_

#include <string>

using namespace std;

/**
 * \class MEMS_Scanner
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 01.03.19
 *
 * Description:
 * @n This Class is an Enumeration to store the OCT Type Configuration.
 */
class OCTType{
public:
	enum TypeEnum {NOT_CONFIGURED, SPECTROMETER, SSOCT};
	static const string octTypeStrVal[3];
};

/**
 * \class MEMS_Scanner
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 01.03.19
 *
 * Description:
 * @n This Class is an Enumeration to store the Running Mode Configuration.
 */
class RunningMode{
public:
	enum ModeEnum {IDLE, RUN};
	static const string runModeStrVal[2];
};

/**
 * \class MEMS_Scanner
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 01.03.19
 *
 * Description:
 * @n This Class is an Enumeration to store the Scan ModeConfiguration.
 */
class ScanMode{
public:
	enum ModeEnum {NOT_CONFIGURED, SINGLE_SCAN, CONTINUOUS_SCAN};
	static const string scanModeStrVal[3];
};

/**
 * \class MEMS_Scanner
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 01.03.19
 *
 * Description:
 * @n This Class is an Enumeration to store the Errors. It contains the Error strings to be sent in case of an error too.
 */
class Errors{
public:
	enum ErrorEnum {NO_ERROR, WRONG_JSON, UNKNOWN_CMD, MISSING_PARAMETER, NO_SCANMODE, WRONG_PARAM_RANGE, TO_MANY_ATTRIBUTES, EMPTY_JSON};
	static const string errorStrVal[8];
};


#endif /* MEMS_INFO_H_ */
