#include "SinSignal.h"
/**
 * @n Name: SinSignal
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 27.11.2018
 *
 * Description:
 * @n Constructor to create a SinSignal object and initializes the important members. A bind must still be performed in
 * order to output the sine waveform.
 */
SinSignal::SinSignal( float freq, float phase, float amp, float offset, float timeIncrement) :
	Signal(freq, timeIncrement, offset),
	mPhase(phase/180*MYPI),
	mw(2.0f*3.14159265*mFrequency),
	mAmplitude(amp)
{
	mStartValue = this->calcNextValue(0);
}

/**
 * @n Name: calcNextValue
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 27.11.2018
 *
 * Description:
 * @n This method calculate the sine waveform at a given point in time. This time is calculated with the timeIncrement
 * and the actual Time index iTime.
 * The STM32f405 contains a floating point unit. So in order to improve performance a floating point algorithm is used
 * to calculate the sine waveform.
 */
float SinSignal::calcNextValue(unsigned long int iTime){
	float val = mAmplitude * sinf(mw*mTimeIncrement*iTime + mPhase);
	return val + mOffset;
}

