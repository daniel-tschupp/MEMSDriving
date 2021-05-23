/*
 * ESP8266.h
 *
 *  Created on: 20.04.2020
 *      Author: dnt
 */

#ifndef HW_ESP8266_H_
#define HW_ESP8266_H_

#include <USARTCom.h>
#include <string>

/**
 * \class ESP8266
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 22.04.20
 *
 * Description:
 * @n This delivers the means to use the ESP8266 Wifi-Module. The communication to the module is
 * via UART. Therefore a uart handle is needed to construct the object. For this class to work the
 * inherited method: addCharacterToBuffer must be called inside a interrupt service routine.
 * If this is done, one can send and receive data via TCP with the following methods: sendData, checkForData.
 * The checkForData method should be called in an interval of about 10-100ms.
 */
class ESP8266 : public USARTCom{
public:
	/** \brief Constructor for ESP8266 objects.
	 * @param name	Name of the module needed for identification.
	 * @param uart	Uart handle pointer.*/
	ESP8266(UART_HandleTypeDef* uart, StaticDataBuffer& rxBuffer);
	/** \brief Destructor for ESP8266 objects. */
	virtual ~ESP8266();

	/** \brief checkESP sends a AT command to verify the Module is ready for communication. */
	void checkESP(void);

	/** \brief Method to send data to Client 0 through the ESP TCP Connection.
	 * @param data		String to be send. */
	void sendData(std::string data);
	/** \brief checkForData checks if a complete data transmission has been received.
	 *
	 * @param data	The string variable the data will be written to. If no data was received
	 * data will be an empty string.
	 */
	std::string checkForData(void);

	/** \brief Method for log in an access point..
		 * @param APName	Name of access point.
		 * @param AOOW		Password of acces point.*/
	void wifiLogin(std::string APName, string APPW);

private:
	std::string extractData(std::string data);

	enum class ESPState
	{
		WaitForWifiConnection,
		WaitForIP,
		AllowMultipleConnections,
		InitServer,
		WaitForClient,
		Communication,
		Error,
		SetupESPAsWifiDevice,
		WifiSetLoginDetails,
		SetupComplete,
		SetupDisconnectWifi
	};
	ESPState mState;
	std::string mAPName;
	std::string mAPPW;
	void reset(void);
};

#endif /* HW_ESP8266_H_ */
