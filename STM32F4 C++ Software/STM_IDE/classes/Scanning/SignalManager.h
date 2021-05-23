#ifndef SIGNALCONTAINER_H_
#define SIGNALCONTAINER_H_

extern "C"{
#include "dac.h"
}
#include "Signal.h"
#include "AScanTrigger.h"
#include "Signal.h"
#include "ConstantSignal.h"
#include "MEMS_Info.h"

/**
 * \class SignalManager
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 01.03.19
 *
 * Description:
 * @n This class contains everything needed to generate the output signals. Furthermore it stores all the state data of
 * the signals. It contains method to create new signals too. For simplicity all the members of this class aren't
 * encapsulated.
 */
class SignalManager
{
public:
	shared_ptr<Signal> mpXSig;		//!< \brief Smart pointer to the X-Channel signal object.
	shared_ptr<Signal> mpYSig;		//!< \brief Smart pointer to the Y-Channel signal object.
	shared_ptr<Signal> mpFastAxis;		//!< \brief Smart pointer to the fast axis signal object.
	shared_ptr<Signal> mpSlowAxis;		//!< \brief Smart pointer to the slow axis signal object.
	shared_ptr<AScanTrigger>mpAScanTriggerPico;	//!< \brief Smart pointer to the AScan trigger object.
	shared_ptr<AScanTrigger>mpAScanTriggerBNC;	//!< \brief Smart pointer to the AScan trigger object.
	shared_ptr<IFilter> mpXFilter;		//!< \brief Smart pointer to the X-Channel filter object.
	shared_ptr<IFilter> mpYFilter;		//!< \brief Smart pointer to the Y-Channel filter object.
	
	uint32_t mSigUpdateFrequency;		//!< \brief Frequency of the DAC readout.
	uint32_t mStopCount;			//!< \brief Number of the A-Scan-Trigger impulses.
	uint32_t miDacSignal;			//!< \brief Actual DAC signal counter.

	float mXVal;				//!< \brief Actual X-Channel analog value.
	float mYVal;				//!< \brief Actual Y-Channel analog value.
	bool mXSigRamp;				//!< \brief Indicator if X-Channel signal starting value is reached.
	bool mYSigRamp;				//!< \brief Indicator if Y-Channel signal starting value is reached.

	OCTType::TypeEnum mOCTType;		//!< \brief Enumeration storing the configured OCT Type (SSOCT, SPECTROMERTER) obsolete the moment
	RunningMode::ModeEnum mRunningMode;	//!< \brief Enumeration storing the run state.
	ScanMode::ModeEnum mScanningMode;	//!< \brief Enumeration storing the configured Scan Mode (Single Scan, continuous scans)
	Errors::ErrorEnum mErrors;		//!< \brief Enumeration storing the actual error state.
	string mXAxisSigType;			//!< \brief Stores the actual X-Channel signal type as string
	string mYAxisSigType;			//!< \brief Stores the actual Y-Channel signal type as string
	uint32_t mAScansPerBScan;		//!< \brief Variable to store the configured amount of A scans per B scan.
	uint32_t mBScansPerCScan;		//!< \brief Variable to store the configured amount of B scans per C scan.
	uint32_t mTrigStopCount;		//!< \brief Variable to store the number of Trigger signal until the trigger will be stopped.

	/** \brief Constructor to create a SignalManager object. Two Constant signals at 1.65V will be created
         *
         * @param       sigUpdateFreq       Frequency of the signal update interrupt to calculate the time between two calls.
         */
	SignalManager(uint32_t sigUpdateFreq);

	/** \brief Method to set a new X-Channel signal without changing the corresponding filter.
         *
         * @param       sig       Shared pointer to a signal object.
         */
	void setNewXSignal(std::shared_ptr<Signal> sig);

	/** \brief Method to set a new X-Channel signal with a new X-Channel filter.
         *
         * @param       sig       Shared pointer to a signal object.
         * @param       filter    Shared pointer to a filter object.
         */
	void setNewXSignal(std::shared_ptr<Signal> sig, shared_ptr<IFilter> filter);

	/** \brief Method to set a new Y-Channel signal without changing the corresponding filter.
         *
         * @param       sig       Shared pointer to a signal object.
         */
	void setNewYSignal(std::shared_ptr<Signal> sig);

	/** \brief Method to set a new Y-Channel signal with a new Y-Channel filter.
         *
         * @param       sig       Shared pointer to a signal object.
         * @param       filter    Shared pointer to a filter object.
         */
	void setNewYSignal(std::shared_ptr<Signal> sig, shared_ptr<IFilter> filter);

	/** \brief Method to determine the Fast axis and set the Fast and Slow axis pointers accordingly. */
	void determineFastAxis(void);
};

#endif /* SIGNALCONTAINER_H_ */
