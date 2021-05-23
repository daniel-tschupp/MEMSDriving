/*
 * MEMS_Info.cpp
 *
 *  Created on: 11.02.2019
 *      Author: tud1
 */
#include "MEMS_Info.h"

const string Errors::errorStrVal[8] =  { 	"NoError",
						"WrongJSON",
						"UnknownCMD" ,
						"MissingParameter",
						"ScanModeNotConfigured",
						"WrongParameterRange",
						"ToManyAttributes",
						"EmptyJSON"}; // stored in Flash

const string OCTType::octTypeStrVal[3] = { 	"NotConfigured",
						"SpectrometerOCT",
						"SSOCT" }; // stored in Flash

const string RunningMode::runModeStrVal[2] = { 	"Idle",
						"Run" }; // stored in Flash

const string ScanMode::scanModeStrVal[3] = { 	"NotConfigured",
						"SingleScan",
						"ContinuousScan" }; // stored in Flash

