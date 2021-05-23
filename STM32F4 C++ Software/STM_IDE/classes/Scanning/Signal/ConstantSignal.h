#ifndef CONSTANT_SIGNAL_H_
#define CONSTANT_SIGNAL_H_

extern "C" {
	#include "stm32f4xx_hal.h"
	#include "stm32f4xx.h"
	#include "dac.h"
}
#include "global_definitions.h"
#include "Signal.h"

/**
 * \class ConstantSignal
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 27.11.18
 *
 * Description:
 * @n This class implements the methods to generates a constant signal waveform.
 */
class ConstantSignal : public Signal{
	private:
		float mPeriod;		// Length of the constant signal
		float mValue;		// Value of the constant signal
		
	public:
		/** \brief Constructor to create a SinSignal object.
		 *
		 * @param	period		Length of the sine singal.
		 * @param	value		Value of the constant singal.
		 * @param	offset		DC-Offset of the sine signal.
		 * @param	timeIncrement	Time increment for each time index.
		 */
		ConstantSignal(float period, float value, float offset, float timeIncrement);

		/**
		 * \brief This method calculates the amplitude of a sine signal at a given point in time.
		 *
		 * @param	iTime	Time index for time calculation.
		 *
		 * @return	The signal value at the given time index.
		 */
		float calcNextValue(unsigned long int iTime) override;
};

#endif
