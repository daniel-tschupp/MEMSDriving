/** Copyright is at the end */
#define SCANNER_APP
//#define ESP_DIRECT
//#define ESP_ECHO

extern "C" {
	void dacInterrupt(void);
	void aScanTriggerInterruptTim1(void);
	void aScanTriggerInterruptTim3(void);
	void uart3Interrupt(void);
	void uart2Interrupt(void);
	void tim14Interrupt(void);
}
#include "global_definitions.h"

#ifdef SCANNER_APP
#include <memory>
#include "MEMSScanner.h"
/* global variables ---------------------------------------------------------*/
std::unique_ptr<MEMS_Scanner> pmems;

/* global function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

int main(void){
	
	HAL_Init();
	__disable_irq();
	SystemClock_Config();

	pmems.reset(new MEMS_Scanner());
	pmems->appExecution();
}

/** \brief This is a Wrapper function to wrap be able to call a MEMS_Scanner method out of a interrput service Routine. */
void aScanTriggerInterruptTim1(void){
	pmems->isrAScanTriggerPico();
}

/** \brief This is a Wrapper function to wrap be able to call a MEMS_Scanner method out of a interrput service Routine. */
void aScanTriggerInterruptTim3(void){
	pmems->isrAScanTriggerBNC();
}
/** \brief This is a Wrapper function to wrap be able to call a MEMS_Scanner method out of a interrput service Routine. */
void dacInterrupt(void){
	pmems->isrDAC();
}

/** \brief This is a Wrapper function to wrap be able to call a MEMS_Scanner method out of a interrput service Routine. */
void uart3Interrupt(void){
	pmems->isrUSBCom();
}

/** \brief This is a Wrapper function to wrap be able to call a MEMS_Scanner method out of a interrput service Routine. */
void uart2Interrupt(void){
	pmems->isrWifiCom();
}

/** \brief This is a Wrapper function to wrap be able to call a MEMS_Scanner method out of a interrput service Routine. */
void tim14Interrupt(void){
	pmems->regularChecks();
}

#endif


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */



#ifdef ESP_ECHO
#include "gpio.h"
#include "ESP8266.h"
std::shared_ptr<ESP8266> pESPCom;
StaticDataBuffer espComBuffer = {0,0,{0}};
void espTCPServerEcho(void){
	MX_GPIO_Init();
	MX_USART2_UART_Init();
	pESPCom = std::make_shared<ESP8266>(&huart2, espComBuffer);
	__enable_irq();

	while(1){
			bool doDelay = true;
			string rec = pESPCom->checkForData();
			if(rec != ""){
				doDelay = false;
				pESPCom->sendData(rec);
				//if(rec == "changeWifi:CompactOCTNET\n")
				//	pESPCom->wifiLogin("CompactOCTNET", "Optic*111");
				//if(rec == "changeWifi:CompactNET\n")
				//	pESPCom->wifiLogin("CompactNET", "Optic*111");
			}
			if(doDelay){
				HAL_Delay(10);
			}
		}
}
int main(void){
	__disable_irq();
	SystemClock_Config();
	espTCPServerEcho();
	while(1){}
}
void uart2Interrupt(void){
	pESPCom->addCharacterToBuffer();
}
void aScanTriggerInterruptTim1(){}
void aScanTriggerInterruptTim3(){}
void uart3Interrupt(){}
void dacInterrupt(){}
void tim14Interrupt(){}
#endif

#ifdef ESP_DIRECT
#include "gpio.h"
#include "USBCom.h"
#include "USARTCom.h"
std::shared_ptr<USBCom> pHostCom;
StaticDataBuffer hostComBuffer;
std::shared_ptr<USARTCom> pESPCom;
StaticDataBuffer espComBuffer;

void espDirecCommunication(void){
	MX_GPIO_Init();
	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
	pHostCom = std::make_shared<USBCom>(&huart3, hostComBuffer);
	pESPCom = std::make_shared<USARTCom>(espComBuffer);
	pESPCom->init(&huart2);
	__enable_irq();

	while(1){
		bool doDelay = true;
		if(pHostCom->isDataAvailable()){
			doDelay = false;
			char* data = pHostCom->readReceiveBuffer();
			pESPCom->send(data);
			pHostCom->clearReceiveBuffer();
		}
		if(pESPCom->isDataAvailable()){
			doDelay = false;
			char* data = pESPCom->readReceiveBuffer();
			pHostCom->send(data);
			pESPCom->clearReceiveBuffer();
		}
		if(doDelay){
			HAL_Delay(100);
		}
	}

}
int main(void){
	__disable_irq();
	SystemClock_Config();
	espDirecCommunication();
	while(1){}
}
void uart3Interrupt(void){
	pHostCom->addCharacterToBuffer();
}

void uart2Interrupt(void){
	pESPCom->addCharacterToBuffer();
}
void aScanTriggerInterruptTim1(){}
void aScanTriggerInterruptTim3(){}
void dacInterrupt(){}
void tim14Interrupt(){}
#endif

/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  *********************** (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
