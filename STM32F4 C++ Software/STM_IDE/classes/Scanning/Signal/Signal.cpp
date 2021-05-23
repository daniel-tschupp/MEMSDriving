/*
 * Signal.cpp
 *
 *  Created on: 27.11.2018
 *  Author: tud1
 */

#include "Signal.h"

/**
 * @n Name: Signal
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 27.11.2018
 *
 * Description:
 * @n Constructor to create a Signal object. As the Signal class is abstract this constructor can only be used from a
 * sibling.
 */
Signal::Signal(float frequency, float timeIncrement, float offset):
	mFrequency(frequency),
	mTimeIncrement(timeIncrement),
	mOffset(offset),
	mActivateFilter(false),
	miTime(0),
	mActualValue(0.0f),
	mActFilteredValue(0.0f),
	mRampInc(0.0001f),
	mFilterPhase(0),
	mEnableOutput(false)
{}

/**
 * @n Name: bind
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 27.11.2018
 *
 * Description:
 * @n This method makes the connection between the STM API for the physical digital to analog converter interface and
 * the signal class. After this bind method all signal value will be written to the selected pin.
 */
void Signal::bind(std::shared_ptr<IFilter>* ppFilter){
	mActualValue = 0;//this->offset;
	if(ppFilter != NULL)
	{
		if(*ppFilter != NULL){
			mppFilter = ppFilter;
			mActivateFilter = true;
			init();
		}
		else{
			mActivateFilter = false;
			mFilterPhase = 0;
		}
		HAL_Delay(1);
	}
}

/**
 * @n Name: getFilterPhase
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 27.11.2018
 *
 * Description:
 * @n This is a getter-method to extract the phase shift introduced by the filter.
 */
uint32_t Signal::getFilterPhase(void){
	return mFilterPhase;
}

/**
 * @n Name: init
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 27.11.2018
 *
 * Description:
 * @n This method initializes a signal. This is especially important if a filter is activatet to have a defined entry
 * point for the filter. Otherwise you will experiance a transient response.
 */
void Signal::init(void){
	mActFilteredValue = 0.0f;
	mEnableOutput = true;
	this->mFilterPhase = 0;
}

/**
 * @n Name: update
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 27.11.2018
 *
 * Description:
 * @n The update method filters the given value if the filtering is activated. Afterward the analog value is converted
 * to a 12 bit integer value that can be written to the DAC data register. This converted value will then be written
 * to the DAC data register and the value will be written to the DAC with generates the analog value at the pin.
 */
float Signal::filterValue(float actualValue){
	mActualValue = actualValue;
	if(mActivateFilter)
		actualValue = (*mppFilter)->filter(actualValue);
		//actualValue = this->FilterWithBessel6Order(actualValue);
	mActFilteredValue = actualValue;
	return actualValue;
	//if(mEnableOutput)
	//	HAL_DAC_SetValue(mpDAC_Handler, mChannel, mAlignment, actualValue *VAL_TO_DAC);
}

/**
 * @n Name: rampToValue
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 27.11.2018
 *
 * Description:
 * @n This Method calculate the absolute difference between the goal value and the actual value of the signal. If this
 * difference is bigger as the max accepted difference it takes one ramp increment step to reduce this difference.
 */
float Signal::rampToValue(float goalValue){
	if(goalValue > mActualValue)
		mActualValue += mRampInc;
	else
		mActualValue -= mRampInc;
	return this->filterValue( mActualValue );
}
bool Signal::goalValueReached(float goalValue){
	if(ABS(goalValue - mActFilteredValue) > 0.05f)
	{
		return false;
	}
	else
	{
		return true;
	}
}

/**
 * @n Name: rampToValue
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 27.11.2018
 *
 * Description:
 * @n This a getter method that extracts the start value of the singal.
 */
float Signal::getStartValue(void){
	return mStartValue;
}

/**
 * @n Name: rampToValue
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 27.11.2018
 *
 * Description:
 * @n This a getter method that extracts the frequency of the singal.
 */
float Signal::getFrequency(){
	return mFrequency;
}
