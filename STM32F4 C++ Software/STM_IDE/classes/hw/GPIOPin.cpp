#include "GPIOPin.h"

GPIOPin::GPIOPin(GPIO_TypeDef* Port, uint16_t Pin, LogicLevel ll){
	this->Port = Port;
	this->Pin = Pin;
	this->ll = ll;
	this->actValue = 0;
	this->setPinToLogicLow();
}

uint16_t GPIOPin::readPin(void){
	uint16_t PinValue;
	if(this->ll == activeHIGH){
		PinValue = HAL_GPIO_ReadPin(this->Port, this->Pin);
	}
	else{
		PinValue = HAL_GPIO_ReadPin(this->Port, this->Pin)?0:1;
	}
	this->actValue  = PinValue;
	return PinValue;
}

void GPIOPin::setPinToLogicHigh(void){
	this->actValue = 1;
	if(this->ll == activeHIGH){
		HAL_GPIO_WritePin(this->Port, this->Pin, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(this->Port, this->Pin, GPIO_PIN_RESET);
	}
}
void GPIOPin::setPinToLogicLow(void){
	this->actValue = 0;
	if(this->ll == activeHIGH){
		HAL_GPIO_WritePin(this->Port, this->Pin, GPIO_PIN_RESET);
	}
	else{
		HAL_GPIO_WritePin(this->Port, this->Pin, GPIO_PIN_SET);
	}
}
uint8_t GPIOPin::getActValue(void){
	return this->actValue;
}

void GPIOPin::togglePin(void){
	if(this->actValue == 0){
		this->setPinToLogicHigh();
	}
	else{
		this->setPinToLogicLow();
	}
}

Navi::Navi(GPIOPin* jUp, GPIOPin* jDown, GPIOPin* jRight, GPIOPin* jLeft, GPIOPin* jCenter){
	this->jUp = jUp;
	this->jDown = jDown;
	this->jRight = jRight;
	this->jLeft = jLeft;
	this->jCenter = jCenter;
}

uint8_t Navi::isAnyPressed(void){
	return 	(	this->jCenter->getActValue() |
			this->jUp->getActValue()|
			this->jDown->getActValue() |
			this->jRight->getActValue() |
			this->jLeft->getActValue()	);
}
