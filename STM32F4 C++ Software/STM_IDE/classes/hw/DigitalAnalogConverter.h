#ifndef HW_DIGITALANALOGCONVERTER_H_
#define HW_DIGITALANALOGCONVERTER_H_

#include <stm32f4xx_hal.h>
#include <dac.h>


/**
 * \class DigitalAnalogConverter
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 14.12.18
 *
 * Description:
 * @n This class is provides the functionality to handle a digiteal analog converter on a microcontroller unit.
 * At the moment it's capable of doing single conversions.
 */
class DigitalAnalogConverter
{
	DAC_HandleTypeDef* mpDAC_Handler;		//!< Handle to the used DAC output.
	uint32_t mChannel;				//!< Number of the used DAC output channel.
	uint32_t mAlignment;				//!< Alignment of data in the DAC data register.
	float mActualValue;				//!< Actual analog value.
	float mOffset;					//!< Fix offset as analog value for the DAC.
	const float cVAL_TO_DAC = 1241.212121;		//!< Value to match the 3.3Volt output to the range of 4096 (12bit) (old: 1321.29032258f)
public:
	/**
	  * \brief This is the constructor for an digital to analog converter object.
	  *
	  * @param      pDAC_Handler	Pointer to the DAC STM API.
	  * @param	channel		Number of the used DAC channel.
	  * @param	alignment	Config data alignment in the DAC data register.
	  */
	DigitalAnalogConverter(DAC_HandleTypeDef* pDAC_Handler, uint32_t channel, uint32_t alignment);
	DigitalAnalogConverter(DAC_HandleTypeDef* pDAC_Handler, uint32_t channel, uint32_t alignment, float offset);

	/** \brief Virtual Destructor of the DigitalAnalogConverter object. */
	virtual ~DigitalAnalogConverter();

	/**
	  * \brief This is the constructor for an digital to analog converter object.
	  *
	  * @param      analogValue	The analog Value to be set.
	  */
	void setNewAnalogValue(float analogValue);

	/**
	 * \brief Getter method for the actual analog value.
	 *
	 * @return	Last set analog value of the DAC.
	 */
	float getActualAnalogValue(void);
};

#endif /* HW_DIGITALANALOGCONVERTER_H_ */
