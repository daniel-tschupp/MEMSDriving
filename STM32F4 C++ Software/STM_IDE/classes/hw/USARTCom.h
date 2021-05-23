/*
 * USBCom.h
 *
 *  Created on: 20.12.2018
 *      Author: tud1
 */

#ifndef USARTCom_H_
#define USARTCom_H_

#include <stm32f4xx_hal.h>
#include <usart.h>

using namespace std;

#define STATIC_DATA_BUFFER_SIZE 1024
typedef struct {
	uint32_t rxPtrIndex = 0;
	uint8_t rxLock = 0;
	uint8_t uart_rx_buffer[STATIC_DATA_BUFFER_SIZE];
} StaticDataBuffer;

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
class USARTCom {
public:
	/** \brief This constructor method creates a USBCom object. */
	USARTCom(StaticDataBuffer& dataBuffer);

	/** \brief This destructor method destructs the USBCom object. */
	virtual ~USARTCom();

	/** \brief This method reads one character from the FTDI chip and stores this character into the RX-Buffer. a */
	void addCharacterToBuffer(void);

	/** \brief This method initializes the UART communication to fit to the FTDI specification.
	 *
	 * @param 	pUartHandler	Pointer to the HAL library object to control the UART device of the MCU.
	 */
	void init(UART_HandleTypeDef* pUartHandler);

	/** \brief This method returns a pointer to the RX data buffer if data is received.
	 *
	 * @return	Pointer to the RX-Buffer containing the received message.
	 */
	char* readReceiveBuffer(void);

	/** \brief This method clears the RX data buffer to ensure that there's enough space in the buffer to store
	 * the next transmission.
	 */
	void clearReceiveBuffer(void);

	bool isDataAvailable(void);

	/** \brief This method locks the RX buffer. While the RX-Buffer is locked, one cannot do a dataReceived call. */
	void lock(void);

	/** \brief This method unlocks the RX buffer. */
	void unlock(void);

	/** \brief Method to check if the RX buffer is locked (true) or free (false) */
	uint8_t islocked(void);

	/** \brief With this method it's possible to send strings over the USB 2.0 communication.
	 *
	 * @param	str	String to be sent via the USB2.0 connection
	 */
	void send(const char* ptr);
private:
	StaticDataBuffer* mpData;
	UART_HandleTypeDef* mpUartHandler;	// Pointer to the UART Handler provided by the HAL library.
	bool mObsoleteData;
};

#endif /* USARTCom */
