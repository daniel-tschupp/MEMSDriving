#ifndef MEMSSCANNER_H_
#define MEMSSCANNER_H_

#include <memory>
#include "global_definitions.h"
#include "MEMS_Info.h"
#include "Hardware.h"
#include "SignalManager.h"
#include "SigStateMachine.h"
#include "FIFOQueueISRsafe.h"
#include "JSONChecker.h"
#include "SignalFactory.h"
#include "SimpleJSONParser.h"

using namespace std;

/**
 * \class MEMS_Scanner
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 01.03.19
 *
 * Description:
 * @n This Class is the main class of the MEMS-Mirror MCU Software. It contains a simple Task Scheduler built
 * out of a interrupt safe FIFO-Queue. Every 200ms the Scheduler will check if new Command Messages have been received.
 * If so, the json string will be checked for correctness and afterwards the Information will be extracted. Dependung on
 * the command, the Scheduler will reconfigure the software or create a new signal or start/stop the signal output, as well
 * as the trigger.
 */
class MEMS_Scanner {
protected:
	shared_ptr<Hardware> mpHW;			// Container to control all the necessary HW components
	shared_ptr<SignalManager> mpSigMan;		// Container to control the signals
	shared_ptr<SigStateMachine> mpSigStateMachine;	// Pointer to the State Machine that generates the signals.
	FIFOQueueISRsafe mQueue;			// First-In-First_Out Task queue (simplest scheduler)
	JSONChecker mJSChecker;				// Container object containing all the allowed communication strings and methods to check incomming messages.
	SignalFactory mSigFactory;			// Factory to generate a signal object out of the configuration information inside a json string.

public:
        /** \brief Constructor for MEMS_Scanner object. In this constructor the is the whole program flow. */
		MEMS_Scanner(void);
        
        /** \brief Descructor cleanly remove the MEMS_Scanner object from the memory. */
		virtual ~MEMS_Scanner(void);

		/** \brief This method executes the actual MEMS software. */
		void appExecution(void);
		void taskUSBReceive(void);
		void taskWifiReceive(void);
        /** \brief This method shall be executed when an AScanTrigger interrupt occurs.*/
        void isrAScanTriggerPico(void);
        void isrAScanTriggerBNC(void);

        /** \brief This method shall be executed when a DAC Conversion Timer interrupt occurs.*/
        void isrDAC(void);

        /** \brief This method shall be executed when characters are received from the USB to UART Transceiver of FTDI.*/
        void isrUSBCom(void);

        void isrWifiCom(void);

        /** \brief This method shall be executed every 200ms to add regular tasks to the task queue. */
        void regularChecks(void);

        /** \brief This method interpretes a parsed json string to extract the containing information.
         *
         * @param       jsParser       Reference to a valid json parser object.
         */
        void jsonInterpreter(SimpleJSONParser& jsParser);

        /** \brief This method extracts all the configuration parameters of a MCU_Config command.
         *
         * @param       jsParser       Reference to a valid json parser object.
         */
        void extractMCUConfigInfo(SimpleJSONParser& jsParser);

        /** \brief This method extracts all the configuration parameters of a MCU_Config command.
          *
          * @param       jsParser       Reference to a valid json parser object.
          */
        void extractFilter(SimpleJSONParser& jsParser);

};

#endif /* MEMSSCANNER_H_ */
