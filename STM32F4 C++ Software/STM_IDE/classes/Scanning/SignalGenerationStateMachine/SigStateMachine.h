#ifndef STATEMACHINE_H
#define STATEMACHINE_H

extern "C"{
#include "tim.h"
}
#include <memory>
#include <vector>
#include "MEMS_Info.h"
#include "Hardware.h"
#include "SignalManager.h"

using namespace std;

/** \brief Enumeration to indicate the States of the SigStateMachine */
enum class State { IDLE=0, START, RAMP_UP, PLAY, RAMP_DOWN};

/**
 * \class ISignalStateMachine
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 23.11.18
 *
 * Description:
 * @n This is an Interface for the different States. It ensures every state implements the doStateAction method which
 * @n will be called by the SigStateMachine.
 */
class ISignalStateMachine{
public:
    virtual ~ISignalStateMachine(){}
    virtual void doStateAction(State& actState, shared_ptr<SignalManager> mpSigMan, shared_ptr<Hardware> hw)=0;
};


typedef vector<shared_ptr<ISignalStateMachine>> SigStateVector;

/**
 * \class SigStateMachine
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 01.03.19
 *
 * Description:
 * @n This class implements a State Machine with a map to determine which function to use according to the actual state.
 */
class SigStateMachine
{
private:
    State mActState;
    SigStateVector mSigVec;
    shared_ptr<SignalManager>& mpSigMan;
    shared_ptr<Hardware>& mpHW;

public:
    /** \brief This constructor method creates a signal state machine that controls the different state a signal can have.
    *
    * @param    pSigMan		Signal Manager object to access the signal objects.
    * @param	hw		Object containing the hardware resources to write to the DAC.
    */
    SigStateMachine(shared_ptr<SignalManager>& pSigMan, shared_ptr<Hardware>& hw);

    /** \brief This method sets the actual state. Pay attentions this can affect the proper execution of the signal.
    *
    * @param       newState       State to be set as actual state.
    */
    void setState(State newState);

    /** \brief This method executes the actual state action. Inside this action the actual state may be changed. */
    void executeActualState(void);
};


/******All the different possible States ******************************/

/**
 * \class IdleState
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 18.02.18
 *
 * Description:
 * @n This class provides an idle state that remains in its state
 * and does nothing.
 */
class IdleState : public ISignalStateMachine{
public:
	/** \brief Dummy constructor to create the IdleState */
	IdleState(){}

	/** \brief Dummy destructor to destroy the IdleState */
	~IdleState(){}

	/** \brief This method does the actions of this State
	 *
	 * @param	actState	Reference to the actual state, so the next State can be ajusted.
	 * @param	mpSigMan	Smart pointer to the signal manager object to access the singals.
	 * @param	hw		Smart pointer to the hardware container object to access the hardware.
	 */
	void doStateAction(State& actState, shared_ptr<SignalManager> mpSigMan, shared_ptr<Hardware> hw);
};

/**
 * \class StartSignal
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 18.02.18
 *
 * Description:
 * @n This class provides a means to do Signal start processings.
 */
class StartSignal : public ISignalStateMachine{
public:
	/** \brief Dummy constructor to create the StartSignal */
	StartSignal(){}

	/** \brief Dummy destructor to destroy the StartSignal */
	~StartSignal(){}

    	 /** \brief This method does the actions of this State
    	 *
    	 * @param	actState	Reference to the actual state, so the next State can be ajusted.
    	 * @param	mpSigMan	Smart pointer to the signal manager object to access the singals.
    	 * @param	hw		Smart pointer to the hardware container object to access the hardware.
    	 */
	void doStateAction(State& actState, shared_ptr<SignalManager> mpSigMan, shared_ptr<Hardware> hw);
};
/**
 * \class RampUpState
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 18.02.18
 *
 * Description:
 * @n This class implements a signal state. In it's state it handles the ramp up to the starting position of the signal.
 */
class RampUpState : public ISignalStateMachine{
public:
	/** \brief Dummy constructor to create the RamoUpState */
	RampUpState(){}

	/** \brief Dummy destructor to destroy the RampUpState */
	~RampUpState(){}

	/** \brief This method does the actions of this State
	*
	* @param	actState	Reference to the actual state, so the next State can be ajusted.
	* @param	mpSigMan	Smart pointer to the signal manager object to access the singals.
	* @param	hw		Smart pointer to the hardware container object to access the hardware.
	*/
	void doStateAction(State& actState, shared_ptr<SignalManager> mpSigMan, shared_ptr<Hardware> hw);
};

/**
 * \class SignalPlayState
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 18.02.18
 *
 * Description:
 * @n This class implements a signal state. In it's state it handles the waveform generation of the signal.
 */
class PlaySignalState : public ISignalStateMachine{
public:
	/** \brief Dummy constructor to create the PlaySignalState */
	PlaySignalState(){}

	/** \brief Dummy destructor to destroy the PlaySignalState */
	~PlaySignalState(){}

	/** \brief This method does the actions of this State
	*
	* @param	actState	Reference to the actual state, so the next State can be ajusted.
	* @param	mpSigMan	Smart pointer to the signal manager object to access the singals.
	* @param	hw		Smart pointer to the hardware container object to access the hardware.
	*/
	void doStateAction(State& actState, shared_ptr<SignalManager> mpSigMan, shared_ptr<Hardware> hw);
};

/**
 * \class RampDownState
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 18.02.18
 *
 * Description:
 * @n This class is a Singleton class. It describes a State of the signal and
 * defines the actions for this state.
 */
class RampDownState : public ISignalStateMachine{
public:
	/** \brief Dummy constructor to create the RampDownState */
	RampDownState(){}

	/** \brief Dummy destructor to destroy the RampDownState */
	~RampDownState(){}

	/** \brief This method does the actions of this State
    	 *
    	 * @param	actState	Reference to the actual state, so the next State can be ajusted.
    	 * @param	mpSigMan	Smart pointer to the signal manager object to access the singals.
    	 * @param	hw		Smart pointer to the hardware container object to access the hardware.
    	 */
    	 void doStateAction(State& actState, shared_ptr<SignalManager> mpSigMan, shared_ptr<Hardware> hw);
};


#endif // STATEMACHINE_H
