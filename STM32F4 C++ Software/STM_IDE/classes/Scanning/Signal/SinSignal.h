#ifndef SINSIGNAL_H_
#define SINSIGNAL_H_

extern "C" {
	#include "stm32f4xx_hal.h"
	#include "stm32f4xx.h"
	#include "dac.h"
}
#include "global_definitions.h"
#include "Signal.h"

/**
 * \class SinSignal
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 27.11.18
 *
 * Description:
 * @n This class implements the methods to generate a sine signal waveform.
 */
class SinSignal : public Signal{
	private:
		float mPhase;		// Phase of the signal.
		float mAmplitude;	// Amplitude of the singal.
		float mw;		// Circular frequency of the signal.
	
	public: 
		/** \brief Constructor to create a SinSignal object.
		 *
		 * @param	freq		Frequency of the sine singal.
		 * @param	phase		Phase of the sine singal.
		 * @param	amp		Amplitude of the sine signal.
		 * @param	offset		DC-Offset of the sine signal.
		 * @param	timeIncrement	Time increment for each time index.
		 */
		SinSignal( float freq, float phase, float amp, float offset, float timeIncrement);

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
