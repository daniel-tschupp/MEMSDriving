/*
 * AScanTrigger.cpp
 *
 *  Created on: 06.11.2018
 *  Author: tud1
 */

#include <AScanTrigger.h>

/* 
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 09.11.18
 *
 * Description:
 * @n Constructor to create an object of this class.
 */
AScanTrigger::AScanTrigger(TIM_HandleTypeDef& TimerHandler, uint32_t SignalChannel, uint32_t SignalCountChannel)
	:mpTimerHandler(&TimerHandler),
	 mSignalChannel(SignalChannel),
	 mSignalCountChannel(SignalCountChannel),
	 mTriggerFrequency(0),
	 miTrigger(0),
	 mAScansPerBScan(0)
{}

/* 
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 09.11.18
 *
 * Description:
 * @n Destructor to cleanly destruct the object after when its anymore.
 */
AScanTrigger::~AScanTrigger(){}

/* 
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 09.11.18
 *
 * Description:
 * @n This method initializes the PWM-Timer. In order to do this
 * it calculates to where the timer has to count to get the right
 * frequency. Afterwards it calls the apropriate API functions (stm c-functions)
 * It also enables the counter interrupt.
 */
void AScanTrigger::initTrigerSignal(uint32_t Frequency, uint32_t BusCLK){
	mTriggerFrequency = Frequency;
	mBusCLK = BusCLK;
	volatile unsigned int arrValue = (unsigned int)((BusCLK / mTriggerFrequency) - 1);// 12000-1;//000 / pwmFreq; 60MHz

	__HAL_TIM_SET_AUTORELOAD(mpTimerHandler, arrValue);

	HAL_TIM_OC_Init(mpTimerHandler);
	__HAL_TIM_SET_COMPARE(mpTimerHandler, mSignalCountChannel, arrValue/2);

	HAL_TIM_PWM_Init(mpTimerHandler);
	__HAL_TIM_SET_COMPARE(mpTimerHandler, mSignalChannel, arrValue/2);

	mpTimerHandler->Instance->CNT = 0;
}

/* 
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 09.11.18
 *
 * Description:
 * @n This method sets the actual counting value of the timer as well as 
 * of the trigger counter to 0 and starts the counter.
 */
void AScanTrigger::startTriggerSignal(uint32_t aScansPerBScan){
	mAScansPerBScan = aScansPerBScan;
	mpTimerHandler->Instance->CNT = 0;
	miTrigger = 0;
	HAL_TIM_PWM_Start(mpTimerHandler, mSignalChannel);
	HAL_TIM_OC_Start_IT(mpTimerHandler, mSignalCountChannel);
}

/* 
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 09.11.18
 *
 * Description:
 * @n This method stops the PWM as well as the trigger counter interrupt.
 * Furthermore it sets the actual counting value of the timer to 0 and
 * the trigger counter as well.
 */
void AScanTrigger::stopTriggerSignal(void){
	HAL_TIM_PWM_Stop(mpTimerHandler, mSignalChannel);
	HAL_TIM_OC_Stop(mpTimerHandler, mSignalCountChannel);
	miTrigger = 0;
	mpTimerHandler->Instance->CNT = 0;
}

/* 
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 09.11.18
 *
 * Description:
 * @n This method counts the trigger counter. It also initiats the
 * PWM to stop when the desired number of triggers occured.
 */
void AScanTrigger::CountTrigger(void){
	miTrigger++;
	if(miTrigger>=mAScansPerBScan && mAScansPerBScan != 0){
		this->stopTriggerSignal();
	}
}

/* 
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 09.11.18
 *
 * Description:
 * @n This method returns the configured PWM frequency.
 */
uint32_t AScanTrigger::getFrequency(void){
	return mTriggerFrequency;
}
