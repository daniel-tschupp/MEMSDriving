#ifndef GPIOPinDef
#define GPIOPinDef

#include <stm32f4xx_hal.h>
#include "main.h"


/** @brief Enumeration to define if the GPIO Pin is Low active or High active.
*/
enum LogicLevel{ activeLOW, activeHIGH };

/** @brief Class that provides functionalities to control and read general purpose inputs and outputs.
*/
class GPIOPin{
private:
	LogicLevel ll;			/**< Logic level of GPIO Pin. */
	uint8_t actValue;		/**< Actual logic pin level. */
public:
	GPIO_TypeDef* Port;		/**< Port of the GPIO Pin. */
	uint16_t Pin;			/**< Pin number of the GPIO Pin. */

/** @brief Construcs a GPIOPin object. (To use when Pin is defined as Input, for Output use @see getActValue)
	@param Port	[in,out]	Port of the GPIO Pin.
	@param Pin	[in]		Pin number of the GPIO Pin.
	@param ll	[in]		Logic level for the GPIO Pin.
*/
	GPIOPin(GPIO_TypeDef* Port, uint16_t Pin, LogicLevel ll);

/** @brief Reads the GPIO Pin.
	@param void
	@return State of Pin. (Logic level corrected)
*/
	uint16_t readPin(void);

/** @brief Sets the GPIO Pin to logic high.
	@param void
	@return void
*/
	void setPinToLogicHigh(void);

/** @brief Sets the GPIO Pin to logic low.
	@param void
	@return void
*/
	void setPinToLogicLow(void);

/** @brief Toggles the GPIO Pin.
	@param void
	@return void
*/
	void togglePin(void);

/** @brief Gets the actual value of the GPIO Pin. (To use when Pin is defined as Output, for Input use @see readPin)
	@param void
	@return The actual Value. (Logic level corrected)
*/
	uint8_t getActValue(void);
};

/** @brief Container class to group a 5 GPIO pins that build a Navigation stick aka Joystick.
*/
class Navi
{
public:
	GPIOPin* jUp;		/**< GPIOPin object for Joystick "Up". */
	GPIOPin* jDown;		/**< GPIOPin object for Joystick "Down". */
	GPIOPin* jLeft;		/**< GPIOPin object for Joystick "Left". */
	GPIOPin* jRight;	/**< GPIOPin object for Joystick "Right". */
	GPIOPin* jCenter;	/**< GPIOPin object for Joystick "Center". */

/** @brief Constructs container class for a joystick.

	@param jUp	[in,out]	GPIOPin object for Joystick "Up".
	@param jDown	[in,out]	GPIOPin object for Joystick "Down".
	@param jRight	[in,out]	GPIOPin object for Joystick "Right".
	@param jLeft	[in,out]	GPIOPin object for Joystick "Left".
	@param jCenter	[in,out]	GPIOPin object for Joystick "Center".
*/
	Navi(GPIOPin* jUp, GPIOPin* jDown, GPIOPin* jRight, GPIOPin* jLeft, GPIOPin* jCenter);

/** @brief Check if any Joystick motion is pressed. (Debounced)
	@param void
	@return Returns 0 if no Joystick motion is pressed.
*/
	uint8_t isAnyPressed(void);
};

#endif
