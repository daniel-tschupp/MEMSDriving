/*
 * DigitalAnalogConverter.cpp
 *
 *  Created on: 14.12.2018
 *      Author: tud1
 */

#include <DigitalAnalogConverter.h>
#define SAT_DAC(x)	(x <= 0) ? 0 : ( (x >= 4095) ? 4095 : x)
/**
 * @n Name: DigitalAnalogConverter
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 14.12.2018
 *
 * Description:
 * @n Constructor to create a DigitalAnalogConverter object.
 */
DigitalAnalogConverter::DigitalAnalogConverter(DAC_HandleTypeDef* pDAC_Handler, uint32_t channel, uint32_t alignment):
	mpDAC_Handler(pDAC_Handler),
	mChannel(channel),
	mAlignment(alignment),
	mOffset(0.0f),
	mActualValue(0.0f)
{}
DigitalAnalogConverter::DigitalAnalogConverter(DAC_HandleTypeDef* pDAC_Handler, uint32_t channel, uint32_t alignment, float offset):
	mpDAC_Handler(pDAC_Handler),
	mChannel(channel),
	mAlignment(alignment),
	mOffset(offset),
	mActualValue(0.0f)
{}

/**
 * @n Name: ~DigitalAnalogConverter
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 14.12.2018
 *
 * Description:
 * @n Destructor to create a DigitalAnalogConverter object.
 */
DigitalAnalogConverter::~DigitalAnalogConverter() {
	this->setNewAnalogValue(0.0f);
	mpDAC_Handler = NULL;
}

/**
 * @n Name: setNewAnalogValue
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 14.12.2018
 *
 * Description:
 * @n This method calls the STM API (HAL) to set the digital analog converter register and starts a conversion.
 */
void DigitalAnalogConverter::setNewAnalogValue(float analogValue){
	mActualValue = analogValue;
	int dacVal = (mActualValue+mOffset) * cVAL_TO_DAC;
	dacVal = SAT_DAC(dacVal);
	HAL_DAC_SetValue(mpDAC_Handler, mChannel, mAlignment, dacVal);
}

/**
 * @n Name: getActualAnalogValue
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 14.12.2018
 *
 * Description:
 * @n Getter method that returns the last set digital analog conversion value.
 */
float DigitalAnalogConverter::getActualAnalogValue(void){
	return mActualValue;
}
