#ifndef SIGNAL_H_
#define SIGNAL_H_

extern "C" {
	#include "stm32f4xx_hal.h"
	#include "stm32f4xx.h"
	#include "dac.h"
}
#include "global_definitions.h"
#include "IFilter.h"
#include <memory>

#define VAL_TO_DAC	1321.29032258f	// 4096/3.1

/**
 * \class Signal
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 27.11.18
 *
 * Description:
 * @n This class is a template class. It can be used to inherrite from and contains the default member variables and
 * methods a signal generating class needs.
 */
class Signal{
	protected:
		float mFrequency;			//!< Frequency of the signal.
		float mTimeIncrement;			//!< Time increment per time index of the time index counter.
		unsigned long int miTime;		//!< Actual time index counter.
		float mActualValue;			//!< Actual value of the signal.
		float mActFilteredValue;
		float mRampInc;				//!< Step increment for up and down ramping to the signal start value.
		std::shared_ptr<IFilter>* mppFilter;			//!< Pointer to the used filter. May be NULL if no filter is used.
		uint32_t mFilterPhase;			//!< Phase offset generated by the filter.
		bool mEnableOutput;

	public:
		float mOffset;				//!< DC offset of the signal.
		bool mActivateFilter;			//!< Variable to activate/deactivate the signal filter.
		float mStartValue;			//!< Start value of the signal.

		/** \brief Constructor to create a Signal object.*
		 * @param	frequency	Frequency of the sine singal.
		 * @param	timeIncrement	Time increment for each time index.
		 * @param	offset		DC-Offset of the sine signal.
		 */
		Signal(float frequency, float timeIncrement, float offset);

		/** \brief Virtual Destructor of the signal object. This is needed to be able to delete siblings. */
		virtual ~Signal(void){ }

		/**
		  * \brief This method binds the signal to a physical digital analog converter and its corresponding
		  * pin.
		  *
		  * @param      pDACHandler	Pointer to the DAC STM API.
		  * @param	channel		Number of the used DAC channel.
		  * @param	alignment	Config data alignment in the DAC data register.
		  * @param	pfilter		Pointer to the used signal filter object.
		  */
		virtual void bind(std::shared_ptr<IFilter>* pFilter);

		/**
		 * \brief This method writes the actual signal value to the DAC data register and triggers the output.
		 * If the filtering is activated it calculates the filtered signal value and uses this calculated value
		 * to write into the DAC data register.
		 *
		 * @param	actualDACValue	The actual analog value which shall be written to the DAC register.
		 */
		virtual float filterValue(float actualDACValue);

		/**
		 * \brief This method can be used to ramp to a goal analog value. It applies on increment step towards
		 * the goal value and returns a boolean value that indicates if the goal value is reached.
		 *
		 * @param	goalValue	Goal value to ramp to.
		 *
		 * @return	Indicator if the goal value is already reached.
		 */
		virtual float rampToValue(float goalValue);
		virtual bool goalValueReached(float goalValue);

		/**
		 * \brief This method initializes the Signal. This means if there's a filter
		 * activated it simulates the signals once with the filter and once
		 * without to find the phase and the damping. furthermore it iterates
		 * to the chosen offset. This is a must, because otherwise the filter
		 * goes crazy.
		 */
		virtual void init(void);

		/**
		 * \brief This method calculates the next analog value of the signal. Its a purely virtual method because
		 * each sibling must implement this method on its own.
		 *
		 * @param	iTime	Time index counter for the actual time calculation.
		 *
		 * @return	Analog value for the given time index.
		 */
		virtual float calcNextValue(unsigned long int iTime)=0;//{}

		/**
		 * \brief Getter-method to extract the signal frequency.
		 *
		 * @return	Actual signal frequency.
		 */
		virtual float getFrequency();

		/**
		 * \brief Getter-method to extract the start value.
		 *
		 * @return	Start value of the signal. (At time index = 0)
		 */
		virtual float getStartValue(void);

		/**
		 * \brief This method simulates two periods of the signal with and without the filter. Through this
		 * simulation the phase shift can be calculated.
		 *
		 * @return	The calculated phase shift induced by the filtering.
		 */
		virtual uint32_t getFilterPhase(void);
};

#endif
