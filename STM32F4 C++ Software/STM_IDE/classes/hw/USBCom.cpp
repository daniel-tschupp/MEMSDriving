/*
 * USBCom.cpp
 *
 *  Created on: 20.12.2018
 *      Author: tud1
 */

#include <USBCom.h>

/**
 * @n Name: USBCom
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n Empty Constructor, doesn't initialize the USB Communication.
 */
USBCom::USBCom(UART_HandleTypeDef* uart, StaticDataBuffer& dataBuffer): USARTCom(dataBuffer)
{
	this->init(uart);
}

/**
 * @n Name: ~USBCom
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 02.03.18
 *
 * Description:
 * @n Empty descructor
 */
USBCom::~USBCom() {}

/**
 * @n Name: sendACK
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 02.03.18
 *
 * Description:
 * @n Method to send a short acknowlegement string "ACK".
 */
void USBCom::sendACK(void){
	this->sendString("ACK");
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
void USBCom::sendString(std::string str){
	const char* ptr = str.c_str();
	this->send(ptr);
}
