/*
 * USBCom.h
 *
 *  Created on: 20.12.2018
 *      Author: tud1
 */

#ifndef COMMUNICATION_USBCOM_H_
#define COMMUNICATION_USBCOM_H_

#include <string>
#include "USARTCom.h"

using namespace std;

#define USB_DATA_BUFFER_SIZE 1024

/**
 * \class USBCom
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 02.03.19
 *
 * Description:
 * @n This class provides easy access to the USB 2.0 communication. In hardware the USB plug is connected to a FTDI chip
 * @n that implements the USB 2.0 communication stack as well as a external buffer. The FTDI chip communicates via UART with
 * @n the microcontroller. This class ensures that a complete package is received. This is done through locking the class while
 * @n receiption an periodically unlocking the class. If no character is received between two cycles, then the communication
 * @n package is complete. The actual buffer size can be defined by the USB_RX_BUFFER_SIZE constant above. The speed of the
 * @n connection is limited to 12MHz by the UART bus clock of the microcontroller. This results in a transfer rate of
 * @n approximately 5Mbit/s where 8 out of 11 bits are data bits.
 */
class USBCom : public USARTCom{

public:
	/** \brief This constructor method creates a USBCom object. */
	USBCom(UART_HandleTypeDef* uart, StaticDataBuffer& dataBuffe);

	/** \brief This destructor method destructs the USBCom object. */
	virtual ~USBCom();

	/** \brief With this method it's possible to send strings over the USB 2.0 communication.
	 *
	 * @param	str	String to be sent via the USB2.0 connection
	 */
	void sendString(std::string str);

	/** \brief Method to send a short "ACK" string. */
	void sendACK(void);
};

#endif /* COMMUNICATION_USBCOM_H_ */
