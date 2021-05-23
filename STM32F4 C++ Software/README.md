## Overview over Documentation:
*	[Prerequisits](#Prerequisites)
*	[Main structure](#MainStructure)
*	[Hardware](#Hardware)
*	[Signal Generation](#SignalGeneration)
*	[Communication](#Communication)

_Note: When this README.md file is displayed on GitLab, the images won't be displayed.
To see the whole file, please clone the repo and execute: "$doxygen doxyfile"
on tht cmd line. Then enter "$firefox doxygen/html/index.html" 
Make sure you met the prerequisits._
 
---

## <a name="Prerequisites">Prerequisits to work on this Project:</a>

---

For Linux:
* git
* Atollic True Studio
* CubeMX
* doxygen (Download & compile the latest version from: https://gitlab.com/pages/doxygen/tree/master )
* graphviz

For Windows:
* git
* Atollic True Studio or Keil uVision
* CubeMX
* doxygen Frontend
* graphviz that works with doxygen frontend
* cygwin with doxygen and graphviz

---

## <a name="MainStructure">Main structure of the software:</a>

---

The Software is build up into 2 parts:
* Foreground (Time critical)
* Background (still realtime but not time critical)

![alt text](dc/daa/classMEMS__Scanner__coll__graph.svg "MEMSScanner")
!
### Foreground
The foreground only consists of interrupt service routins. The main part is the signal generation for the MEMS mirrors (Timer 13). 
But there are also some timer interrupts for event generation for the background part of the software.

Actual tasks of the foreground:
*	x-axis and y-axis signal generation (timer 13)
*	a-scan trigger generation (timer 1: CH1=PWM, CH2=CountCycles)
*	SysTick (1ms)
*	Regulare 100ms Event for background

### Background
The background conserns with all the things that are not time critical. Though it should be fast enough so the user of the software doesn't notice any delays.
Actual the tasks of the background are:
*	Initialisation
*	Communication to Host
*	Protocol parsing
*	Scan initiation
*	GPIO handling


### Build up
The MEMS_Scanner object contains a minimalistic scheduler. This is achived with a Interrupt safe FIFO-Queue and Interrupts that inserts tasks into this queue. This are the typical background activities.

To check the validity of incoming messages/commands it contains an JSONChecker object.

To create new Signal it contains a SignalFactory object.

To access the hardware components of the microcontroller like GPIO, DAC, UART etc. it contains a Hardware object.

To actually write the signals to the outputs (foreground task) it contains furthermore a SigStateMachine object.

### <a name="Hardware">Hardware</a>

To access the hardware (like GPIOs etc.) there exist a class Hardware. This class contains objects for all the used hardware. This is in particular:
* GPIO Enable HV Amplifier
* GPIO Enable HV Generation
* UART - USB Communication
* DAC1 for Y axis signal creation
* DAC2 for X axis signal creation
* SPI - Bluetooth Communication (not yet implemented)

UML:

![alt text](df/d85/classHardware__coll__graph.svg "Hardware")

### SignalManager
To have things well sorted inside the main MEMS_Scanner class, there exists a SignalManager object. It contains everything about the signal generation as well as the actual state, values, mode etc. of the signal generation.

UML:

![alt text](d6/d23/classSignalManager__coll__graph.svg "SignalManager")


### FIFOQueueISRsafe
As described above the microcontroller software generates Events through interrupts. Those interruptse add Tasks to this interrupt save task queue. This provides a minimal FIFO scheduler functionality which is great to add additional software components.
But for the actual generation of the Signals in a 100kHz rate it's to slow. That's why the generation of the signals is handed separately, direct in the 100kHz timer interrupt.
All the other tasks of the software that are less time critical are handled via this queue. Ath the moment this includes the following tasks:
* Communication
* JSON string checking
* JSON string parsing
* Microcontroller configuration
* Signal configuration

UML:

![alt text](d1/daf/classFIFOQueueISRsafe__coll__graph.svg "FIFOQueue")

---

## <a name="SignalGeneration">Signal-Generation</a>

---

### <a name="StateMachine">SigStateMachine</a>
A state machine called by the interrupt of timer 13 ensures that the signal is generated correctly. The structure of this
state machine can be seen below:

![alt text](d5/d5c/classSigStateMachine__coll__graph.svg "IState Attribute")

As can be seen, the state machine contains a member that is an interface for a state. In the following UML you can see all the classes that 
implements this interface.
![alt text](de/d8a/classIState__inherit__graph.svg "IState")

---

### <a name="Signal">Signal</a>

There are some common tasks that a signal must be able to perform, regardless of how its form looks like. To represent that in software a
parrent class Signal is implemented. This parrent class implements:
* Filtering the Signal if needed
* Ramp smoothly to starting position of the signal
* simulating a filtered signal to calc phase shift and damping

The next UML shows this parrent class and all it's siblings:

![alt text](dc/de3/classSignal__inherit__graph.svg "Signal")

As can be seen, the siblings implement primarily the pure virtual calcNextValue() - method of the parrent. This method implements the different waveform calculation.

At the moment three waveforms are implemented:
* ConstantSignal
* SinSignal
* TriangleSignal

Furthermore the parrent class contains a filter interface. This is for easy implementing other filter alorithms. The UML shows how it works:

![alt text](d4/de3/classIFilter__inherit__graph.svg "IFilter")

As for now only one filter algorithm is implemented and that is a Bessel6thOrder300Hz filter with a cut of frequency of 300Hz. The filter implementation primarily implements the filter() - method. This method must be able to calculate the next filtered value out of the new theoretical value. Because the Signals need a filtering object there is also a NoFilter object with just returns the given value.

### SignalFactory

To actually create the singal object there exists a factory. The Factory includes a map that generates the correcte signal according to the string given by the json command string. Refere to the UML to see the factory methods provided:

![alt text](d8/dc6/classSineSignalFactory__inherit__graph.svg "SineFactory")
![alt text](d5/d3a/classTriangleSignalFactory__inherit__graph.svg "TriangleFactory")
![alt text](d4/df7/classConstantSignalFactory__inherit__graph.svg "ConstantFactory")

---

## <a name="Communication">Communication</a>

---

The communication to the microcontroller can be via Bluetooth or USB type C connection. The USB protocoll is only USB 2.0 because of the FTDI Chip that's mountet on the print. Due to the BUS speed of the UART1 of the microcontroller a maximal data rate of about tMbits is possible.
The chosen protocol for the communication is JSON because it's user readable, easy debuggable and also well established. Furthermore it's a lightwidth protocol with a very small amount of overhead.  

### <a name="Protocol">Protocol

#### Definition of the Communication

This chapter describes all the available commands in json format.

Get actual state:

	{
		"GetState"
	}


Actual State Answer:

	{
		"ActState": {
			"OCTType": "SpectrometerOCT",		/* Options: "SpectrometerOCT", "SSOCT" */
			"DACSamplingFrequency": 100000,		/* 10k [Hz] */
			"AScanFrequency": 20000,		/* 1-120'000 [Hz]*/
			"ScanningMode": "SingleScan",		/* Options: "SingleScan", "ContinuousScan", "NotConfigured" */
			"RunningState": "Idle",			/* OneScan, "Run", "Idle" */
			"XAxisSignalType": "SineSignal",	/* Options: "SineSignal","TriangleSignal","ConstantSignal"*/
			"YAxisSignalType": "ConstantSignal",	/* Options: "SineSignal","TriangleSignal","ConstantSignal"*/		
			"AScansPerBScan": 500,			/* Range: 0-65535 */
			"BScansPerCScan": 500,			/* Range: 0-65535 */
			"ErrorState": "NoError"			/* Errors:	NoError, WrongJSON, UnknownCMD, MissingParameter, ScanModeNotConfigured, WrongParameterRange, ToManyAttributes, EmptyJSON */
		}
	}      


Configure microcontroller:

	{
		"MCUConfig": {
			"OCTType": "SpectrometerOCT",	/* Options: "SpectrometerOCT", "SSOCT" */
			"AScanFrequency": 20000,	/* 1-1'000'000 [Hz] */
			"ScanningMode": "SingleScan"	/* Options: "SingleScan", "ContinuousScan" */
		}
	}     

Configure a sine wave:

	{
		"SineSignal": { 
			"OutputStage" : "XAxis", 	/* Options: "XAxis", "YAxis" */
			"Frequency" : 120, 		/* 0.001 - 1500 [Hz] */
			"Amplitude" : 1.5,		/* 0 - 1.5 [V] */
			"Phase": 0,			/* 0-360 [deg] */
			"Offset": 0,			/* -1.5 - 1.5 [V] */
			"Filter": "NoFilter"		/* Options: 	"NoFilter", "Bessel6thOrder300Hz", "MagnMemsIIR10000", "IIR_X", "IIR_Y" */
		}
	}  

Configure a trianglular wave:

	{
		"TriangleSignal": { 
			"OutputStage" : "XAxis",	/* Options: XAxis, YAxis */
			"Frequency" : 200, 		/* 0.01 - 1000 [Hz] */
			"Amplitude" : 1.5,		/* 0 - 1.5 [V] */
			"Phase": 90,			/* 0-360 [deg] */
			"Offset": 0,			/* -1.5 - 1.5 [V] */
			"Width": 0.5,			/* 0 - 1[1] Defines stepness of slopes*/
			"Filter": "IIR_X"		/* Options: 	"NoFilter", "Bessel6thOrder300Hz", "MagnMemsIIR10000", "IIR_X", "IIR_Y" */
		}
	}

Configure a constant signal:

	{
		"ConstantSignal": { 
			"OutputStage" : "XAxis", 	/* Options: XAxis, YAxis */
			"Duration" : 0.05, 		/* 000001 - 15000 [s] */
			"DCValue" : 0,			/* -1.5 - 1.5 [V] */
			"Offset": 0,			/* -1.5 - 1.5 [V] */
			"Filter": "NoFilter"		/* Options: 	"NoFilter", "Bessel6thOrder300Hz", "MagnMemsIIR10000", "IIR_X", "IIR_Y" */
		}
	}

Sending IIR filter parameter:
	
	{
		"SetFilter": {
			"FilterName": "IIR_X",	/* Name of the Filter can be arbitrary*/
			"Fs": 10000.0,		/* Sampling frequency of filter */
			"Gain": 1.1,		/* Gain of the Filter */
			"a0": 1.0,		/* Filter coefficients */
			"a1": 1.0,
			"a2": 1.0,
			"a3": 1.0,
			"a4": 1.0,
			"b0": 1.0,
			"b1": 1.0,
			"b2": 1.0,
			"b3": 1.0,
			"b4": 1.0
		}
	}

Start Signal playing:

	{
		"RunScan"
	}

Stop Signal playing:

	{
		"StopScan"
	}


#### SimpleJSONParser

The SimpleJSONParser class provides the means to decrypt a json string and extract the sent values. For the moment this works only with easy json strings like the ones above, but not with json strings that are more nested. The underlying library of this Parser class is a super lightweith json library called jsmn. It's an open source library that can be found here: https://gitlab.com/NTPsec/ntpsec/tree/master/libjsmn

The following UMS shows the provided method:

![alt text](d2/d2c/classSimpleJSONParser__coll__graph.svg "SimpleJSONParser")


#### JSONChecker

The JSONChecker class provides the means to check if a string is a json string and if it complies
to the defined syntax in the chapter protocoll. It checks the ranges of the given values too.

![alt text](dd/d75/classJSONChecker__coll__graph.svg "JSONChecker")

### <a name="USB_Communication">USB_Communication</a>

The USBCom class provides methods to send strings via UART --> FTDI chip --> USB2.0.
Furthermore in combination with the UART interrupt it fills a receive buffer when characters are received. This buffer can be read via another method provided.

The following block diagram shows the communication path:
![](Website/doc/USBCom.png "USBCom")
\image html USBCom.png

The UML shows all the provided methods: 
![](dc/d3c/classUSBCom__coll__graph.svg "USBCom")

### <a name="Wifi_Communication">Wifi Communication</a>

The following block diagram shows the communication path:

![](Website/doc/WifiCom.png "WifiCom")
\image html WifiCom.png

The UML shows all the provided methods:
![](dd/d7d/classESP8266__inherit__graph.svg "WifiCom")

To use the Wifi Communication, the MCU-Print must be in the enviroment of a Network with the network ID: CompactNET and the standart optolab password.

## ESP8266 used AT commands
* ATE0 (disable Echo)
* AT+RST (reset)
* AT+CWJAP="{}OCT-Scanner","{}Optic111",1,3 (logs into a wifi access point)
* AT+CWMODE=1		(Sets esp mode to client mode only)
* AT+CIPMUX=1 (activates multiple Clients support)
* AT+CIPSERVER=1,5001		(starts server with port: 5001)
* AT+CIPSTO=6000	(sets server timeout to 6000)
* AT+CIPSEND=0,4	(sends message command, de next 4 characters will bi transmittet)
	\item MEMS				(example transmission) 
---

