/*
 * Hardware.cpp
 *
 *  Created on: 25.02.2019
 *      Author: tud1
 */

#include "Hardware.h"
#include <tim.h>
#include <gpio.h>

// Initializing Static Buffers needed for communication
StaticDataBuffer usbBuffer = { 0, 0, {0}};
StaticDataBuffer wifiBuffer = { 0, 0, {0}};
/**
 * @n Name: Hardware
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This constructor method creates a Hardware container object. Pay attention to include new created library functions
 * by cube mx inside here!!!
 */
Hardware::Hardware() {
	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DAC_Init();
	MX_TIM13_Init();
	MX_TIM1_Init();
	MX_TIM3_Init();
	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
	MX_TIM14_Init();

	mpDAC1 = make_shared<DigitalAnalogConverter>(&hdac, DAC1_CHANNEL_1, DAC_ALIGN_12B_R, 1.65f);
	mpDAC1->setNewAnalogValue(0.0f);
	mpDAC2 = make_shared<DigitalAnalogConverter>(&hdac, DAC1_CHANNEL_2, DAC_ALIGN_12B_R, 1.65f);
	mpDAC2->setNewAnalogValue(0.0f);

	mpUSB = make_shared<USBCom>(&huart3, usbBuffer);
	__disable_irq();
	mpWifi = make_shared<ESP8266>(&huart2, wifiBuffer);
	__disable_irq();

	HAL_TIM_OC_Init(&htim14);
	HAL_TIM_OC_Start_IT(&htim14, TIM_CHANNEL_1);

	__enable_irq();
}

/**
 * @n Name: ~Hardware
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n Descructor of the Hardware container object.
 */
Hardware::~Hardware() {
}

/**
 * @n Name: startDACOutputTimer
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This method starts the timer that calls the signal generation state machine and therefore indirectly the signal output.
 */
void Hardware::startDACOutputTimer(void){
	HAL_TIM_Base_Start_IT(&htim13);
}

/**
 * @n Name: setXSignalValue
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This method write the voltage value given by val to the DAC of the x-axis.
 */
void Hardware::setXSignalValue(float val){
	mpDAC2->setNewAnalogValue(val);
}

/**
 * @n Name: setYSignalValue
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This method writes the voltage value given by val to the DAC of the y-axis.
 */
void Hardware::setYSignalValue(float val){
	mpDAC1->setNewAnalogValue(val);
}
