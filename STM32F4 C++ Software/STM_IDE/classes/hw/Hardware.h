/*
 * Hardware.h
 *
 *  Created on: 25.02.2019
 *      Author: tud1
 */

#ifndef HW_HARDWARE_H_
#define HW_HARDWARE_H_

#include <memory>
#include "DigitalAnalogConverter.h"
#include "GPIOPin.h"
#include "ESP8266.h"
#include "USBCom.h"

using namespace std;

/**
 * \class Hardware
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 01.03.19
 *
 * Description:
 * @n This class is a container class that includes all hardware used by the MEMS-Scanner software for easy access.
 * For simplicity all the member variables are public.
 */
class Hardware {
public:
	shared_ptr<DigitalAnalogConverter> mpDAC1;	//!< Smart pointer to the Digital Analog Converter object of channel one.
	shared_ptr<DigitalAnalogConverter> mpDAC2;	//!< Smart pointer to the Digital Analog Converter object of channel two.
	//shared_ptr<GPIOPin> mpEN_Amplification;		//!< Smart pointer to the Digital Input Output Object to control analog amplification enable.
	//shared_ptr<GPIOPin> mpEN_HighVoltage;		//!< Smart pointer to the Digital Input Output Object to control the High Voltage generation enable.
	shared_ptr<USBCom> mpUSB;			//!< Smart pointer to the USB (over UART FTDI chip) communication object to read/sent data.
	shared_ptr<ESP8266> mpWifi;

	/** \brief Constructor that initializes all the hardware. Pay attention the init functions created by cube
	 * are still needed, otherwise it wouldn't be possible to configure the uC with cube! */
	Hardware();

	/** \brief Destructor */
	virtual ~Hardware();

	/** \brief Method to start the DAC output timer that calls the Signal generation state machine. */
	void startDACOutputTimer(void);

	/** \brief Method to set a new value to the DAC output for the x-axis.
	 *
	 * @param	val	The new voltage value that will be converted to an analog value.
	 */
	void setXSignalValue(float val);

	/** \brief Method to set a new value to the DAC output for the y-axis.
	 *
	 * @param	val	The new voltage value that will be converted to an analog value.
	 */
	void setYSignalValue(float val);
};

#endif /* HW_HARDWARE_H_ */
