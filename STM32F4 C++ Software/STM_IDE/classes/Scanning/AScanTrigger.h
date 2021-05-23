#ifndef ASCANTRIGGER_H_
#define ASCANTRIGGER_H_

extern "C"{
#include "tim.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
}


/**
 * \class AScanTrigger
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 09.11.18
 *
 * Description:
 * @n This class can generate a A-Scan-Trigger signal on a defined pin.
 * To do so it uses a PWM capable timer of the micro controller.
 * Furthermore it counts the given triggers and stops after a defined 
 * number of triggers.
 *
 */
class AScanTrigger {
	TIM_HandleTypeDef* mpTimerHandler;      // Pointer to the timer hardware
	uint32_t mSignalChannel;                // Channel for PWM generation
	uint32_t mSignalCountChannel;           // Channel for Trigger counting
	uint32_t mTriggerFrequency;             // PWM frequency
	uint32_t miTrigger;                     // Trigger counter
	uint32_t mAScansPerBScan;               // Number after which the trigger signal stops

public:
	uint32_t mBusCLK;			// Bus Clk of PWM Timer
        /* 
         * \brief Constructor for AScanTrigger object   
         *
         * @param       TimerHandler            Reference to Timer HW
         * @param       SignalChannel           Channel for PWM generation
         * @param       SignalCountChannel      Channel for Trigger counting
         */
	AScanTrigger(TIM_HandleTypeDef& TimerHandler, uint32_t SignalChannel, uint32_t SignalCountChannel);
        
        /** \brief Destructor for AScanTrigger */
	virtual ~AScanTrigger(void);

        /** 
         * \brief Initialisation for PWM Signal
         *
         * @param       Frequency       Frequency of the PWM Signal
         * @param       BusCLK          Frequency of the Bus clock of the PWM module
         */
	void initTrigerSignal(uint32_t Frequency, uint32_t BusCLK);
	
        /** 
         * \brief This method starts the trigger signal
         *
         * @param       aScansPerBScan  With this parameter one defines how many Triggers will be generated.
        */
        void startTriggerSignal(uint32_t aScansPerBScan);
	
        /** \brief This method stops the trigger signal. */
        void stopTriggerSignal(void);   

        /** \brief This method counts triggers. */
	void CountTrigger(void);

        /** \brief This is a getter method to read out the set frequency. */
	uint32_t getFrequency(void);
};

#endif /* ASCANTRIGGER_H_ */
