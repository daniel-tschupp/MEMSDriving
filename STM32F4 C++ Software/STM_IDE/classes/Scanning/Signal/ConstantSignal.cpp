#include "ConstantSignal.h"

/**
 * @n Name: ConstantSignal
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 27.11.2018
 *
 * Description:
 * @n Constructor to create a ConstantSignal object and initializes the important members. A bind must still be
 * performed in order to output the sine waveform.
 */
ConstantSignal::ConstantSignal(float period, float value, float offset, float timeIncrement):
	Signal(1.0f/period, timeIncrement, offset),
	mPeriod(period),
	mValue(value)
{
	mStartValue = mValue + mOffset;
}

/**
 * @n Name: calcNextValue
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 27.11.2018
 *
 * Description:
 * @n This method returns just the chosen value with its offset as it is a constant signal.
 */
float ConstantSignal::calcNextValue(unsigned long int iTime){
	return mValue+mOffset;
}
