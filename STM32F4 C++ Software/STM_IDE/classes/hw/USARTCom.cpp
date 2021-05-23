/*
 * USBCom.cpp
 *
 *  Created on: 20.12.2018
 *      Author: tud1
 */

#include <USARTCom.h>
#include <string.h>

/**
 * @n Name: USBCom
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n Empty Constructor, doesn't initialize the USB Communication.
 */
USARTCom::USARTCom(StaticDataBuffer& dataBuffe): mpData(&dataBuffe), mObsoleteData(false){}

/**
 * @n Name: ~USBCom
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 02.03.18
 *
 * Description:
 * @n Empty descructor
 */
USARTCom::~USARTCom() {}

/**
 * @n Name: init
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 02.03.18
 *
 * Description:
 * @n This method initializes the USBCom object and the UART communication. It enables the input interrupt too.
 */
void USARTCom::init(UART_HandleTypeDef* pUartHandler){
	mpUartHandler = pUartHandler;

	//mpData->rxPtrIndex = STATIC_DATA_BUFFER_SIZE;
	//this->clearReceiveBuffer();

	//HAL_UART_Init(mpUartHandler);
	__HAL_UART_ENABLE_IT(mpUartHandler, UART_IT_RXNE);
	HAL_UART_Receive_IT(mpUartHandler, mpData->uart_rx_buffer, STATIC_DATA_BUFFER_SIZE);
	//mpUartHandler->Instance->CR1 |= USART_CR1_RXNEIE;//  COM_Handler.RXNEIE
}

/**
 * @n Name: addCharacterToBuffer
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 02.03.18
 *
 * Description:
 * @n This method adds a received character to the RX buffer. It has to be called in the interrupt service routine of the
 * UART interrupt.
 */
void USARTCom::addCharacterToBuffer(void){
	this->lock();
	if(mpData->rxPtrIndex < STATIC_DATA_BUFFER_SIZE){
		mpData->uart_rx_buffer[mpData->rxPtrIndex] = mpUartHandler->Instance->DR;
		mpData->rxPtrIndex++;
	}
}

/**
 * @n Name: dataReceived
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 02.03.18
 *
 * Description:
 * @n This method returns a pointer to the character buffer containing the received data.
 */
char* USARTCom::readReceiveBuffer(void){
	char* pData = NULL;
	mObsoleteData = true;
	if(!this->islocked()){
		if(mpData->uart_rx_buffer[0] != 0x00){
			pData = (char*)mpData->uart_rx_buffer;
		}
	}
	return pData;
}

/**
 * @n Name: clearReceivedData
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 02.03.18
 *
 * Description:
 * @n This cmethod clears the received buffer.
 */
void USARTCom::clearReceiveBuffer(void){
	for(uint32_t i = 0; i < mpData->rxPtrIndex; i++)
		mpData->uart_rx_buffer[i] = 0x00;
	mpData->rxPtrIndex = 0;
	this->unlock();
	mObsoleteData = false;
}

bool USARTCom::isDataAvailable(void){
	bool dataAvailable = false;
	if(this->islocked()){
			this->unlock();
	}
	else{
		if(mpData->rxPtrIndex > 0 && !mObsoleteData)
			dataAvailable = true;
	}
	return dataAvailable;
}

/**
 * @n Name: lock
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 02.03.18
 *
 * Description:
 * @n This method locks the RX buffer so nothing can be read while receiving a communication package.
 */
void USARTCom::lock(void){
	mpData->rxLock = 1;
}

/**
 * @n Name: unlock
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 02.03.18
 *
 * Description:
 * @n This method unlocks the RX buffer.
 */
void USARTCom::unlock(void){
	mpData->rxLock = 0;
}

/**
 * @n Name: islocked
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 02.03.18
 *
 * Description:
 * @n This method checks wether the RX buffer is locked.
 */
uint8_t USARTCom::islocked(void){
	return mpData->rxLock;
}

/**
 * @n Name: sendString
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 02.03.18
 *
 * Description:
 * @n Method to send a string via the USB 2.0 connection. The timeout is 100ms.
 */
void USARTCom::send(const char* ptr){
	uint16_t length = strlen(ptr);
	HAL_UART_Transmit(mpUartHandler, (uint8_t*)ptr, length, 100);
}



