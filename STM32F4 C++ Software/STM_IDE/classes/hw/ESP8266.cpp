/*
 * ESP8266.cpp
 *
 *  Created on: 20.04.2020
 *      Author: dnt
 */

#include "ESP8266.h"

/**
 * @n Name: ESP8266
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 22.04.2020
 *
 * Description:
 * @n Constructor that constructs a ESP8266 object. and initializes the uart and $
 * sets up the Soft AP server on the ESP8266 wifi module.
 */
ESP8266::ESP8266(UART_HandleTypeDef* uart, StaticDataBuffer& rxBuffer) :
	USARTCom(rxBuffer),
	mState(ESPState::WaitForWifiConnection)
{
	this->init(uart);
	this->send("ATE0\r\n");
	this->reset();
}

/**
 * @n Name: ~ESP8266
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 22.04.2020
 *
 * Description:
 * @n Destructor that descructsl the ESP8266 object. Is Empty so far.
 */
ESP8266::~ESP8266() {}

/**
 * @n Name: sendData
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 22.04.2020
 *
 * Description:
 * @n To send a data string through the TCP connection, this method can be used.
 */
void ESP8266::sendData(string data){
	if(mState == ESPState::Communication){
		char sendCmd[25];
		sprintf(sendCmd, "AT+CIPSEND=0,%i\r\n",data.length());
		//string sendCmd = "AT+CIPSEND=0," + data.length();
		//sendCmd += "\r\n";
		this->send(sendCmd);
		HAL_Delay(50);

		this->send(data.c_str());
	}
}

/**
 * @n Name: checkForData
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 22.04.2020
 *
 * Description:
 * @n This method checks whether a complete data transmission has been received. In order to
 * do so, it needs to be called regularly. Recommended is a 10-100ms interval. If the interval
 * is bigger, the communication will be slow, with a high lag. If it is to short, it may happen
 * that the packets aren't complete.
 */
string ESP8266::checkForData(void){
	string data = "";

	if(this->isDataAvailable()){
		data.append(this->readReceiveBuffer());

		// Remove leading line breaks
		while(data.c_str()[0] == '\r' && data.c_str()[1] == '\n'){
			data = data.substr(2, data.length()-2);
		}

		if(data.substr(0,7) == "ERROR\r\n")
			mState = ESPState::Error;

		if(data == "0,CLOSED\r\n")
			if(mState != ESPState::SetupDisconnectWifi)
				mState = ESPState::WaitForClient;

		if(data == "WIFI DISCONNECT\r\n")
			if(mState != ESPState::SetupDisconnectWifi)
				mState = ESPState::WaitForWifiConnection;

		switch(mState){
		case ESPState::WaitForWifiConnection:
			if(data == "WIFI CONNECTED\r\n")
				mState = ESPState::WaitForIP;
			data = "";
			break;
		case ESPState::WaitForIP:
			if(data == "WIFI GOT IP\r\n"){
				mState = ESPState::AllowMultipleConnections;
				this->send("ATE0\r\n");
			}
			data = "";
			break;
		case ESPState::AllowMultipleConnections:
			this->send("AT+CIPMUX=1\r\n");
			mState = ESPState::InitServer;
			data = "";
			break;
		case ESPState::InitServer:
			if(data == "OK\r\n"){
				this->send("AT+CIPSERVER=1,5001\r\n");
				mState = ESPState::WaitForClient;
			}
			data = "";
			break;
		case ESPState::WaitForClient:
			if(data == "0,CONNECT\r\n"){
				mState = ESPState::Communication;
			}
			data = "";
			break;
		case ESPState::Communication:
			data = this->extractData(data);
			break;
		case ESPState::Error:
			this->reset();
			mState = ESPState::WaitForWifiConnection;
			data = "";
			break;
		default:
			break;
		}
		this->clearReceiveBuffer();
	}
	return data;
}

void ESP8266::wifiLogin(string APName, string APPW){
	mAPName = APName;
	mAPPW = APPW;
	mState = ESPState::SetupDisconnectWifi;
}

string ESP8266::extractData(string data){
	if(data.length() >= 4){
		if(data.substr(0,4) == "+IPD"){
			string tmp = data.substr(7, data.length()-7); // Cut out +IPD,0,
			int num;
			sscanf(tmp.c_str(), "%i", &num);
			data = data.substr(data.length()-num, num);
		}
		else
		{
			data = "";
		}
	}
	else{
		data = "";
	}
	return data;
}

void ESP8266::reset(void){
	this->send("AT+RST\r\n");
}



