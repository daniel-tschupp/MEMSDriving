#include "SigStateMachine.h"

/**
 * @n Name: SigStateMachine
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This constructor method creates a fully usable SigSTateMachine object. It crates a state vector and initializes it
 * with the following states: IdleState, StartState, RampUpState, PlaySignalState and RampDownState.
 */
SigStateMachine::SigStateMachine(shared_ptr<SignalManager>& pSigCont, shared_ptr<Hardware>& hw):
mActState(State::IDLE),
mpSigMan(pSigCont),
mpHW(hw)
{
	mSigVec.push_back(make_shared<IdleState>());		// Has to match to the State Enum!!!
	mSigVec.push_back(make_shared<StartSignal>());
	mSigVec.push_back(make_shared<RampUpState>());
	mSigVec.push_back(make_shared<PlaySignalState>());
	mSigVec.push_back(make_shared<RampDownState>());
}

/**
 * @n Name: executeActualState
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This Method executes the doStateAction method of the actual state. It accesses the actual state through the
 * actual state enumeration integer value and the corresponding signal state vector (mSigVec). Therefore it is crucial
 * that the State enumeration corresponds to the position of the corresponding state inside the vector.
 */
void SigStateMachine::executeActualState(){
	mSigVec.at((unsigned int)mActState)->doStateAction(mActState, mpSigMan, mpHW);
}

/**
 * @n Name: setState
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n The setState method changes the actual state to another one. But pay attention not to interfere with the actual
 * execution of the signal generation.
 */
void SigStateMachine::setState(State newState){
	mActState = newState;
}

/**
 * @n Name: doStateAction
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This is the doStateAction method of the IdleState. This is basically a do nothing method, because in the IdleState
 * the Signal Generation State Machine shall wait until the signal generation is started.
 */
void IdleState::doStateAction(State& actState, shared_ptr<SignalManager> mpSigMan, shared_ptr<Hardware> hw){
}

/**
 * @n Name: doStateAction
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This is the doStateAction method of the StartSignal state. Here some initial signal start commands can be executed.
 * After one cycle it will change to the RampUpState.
 */
void StartSignal::doStateAction(State& actState, shared_ptr<SignalManager> mpSigMan, shared_ptr<Hardware> hw){
	hw->setXSignalValue(0.0f);
	hw->setYSignalValue(0.0f);
	actState = State::RAMP_UP;
}

/**
 * @n Name: doStateAction
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This is the doStateAction method of the RampUpState. For the MEMS-Mirror it is crucial that the signal has no
 * jumps. Therefore one must ensure a smooth transition from the idle state position in the middle to the signal start
 * position. This method ensures this smooth transition. Once the starting point of the signal is reached the state will
 * change to PlaySignalState.
 */
void RampUpState::doStateAction(State& actState, shared_ptr<SignalManager> mpSigMan, shared_ptr<Hardware> hw){
	mpSigMan->mXVal = mpSigMan->mpXSig->rampToValue(mpSigMan->mpXSig->mStartValue);
	mpSigMan->mXSigRamp = mpSigMan->mpXSig->goalValueReached(mpSigMan->mpXSig->mStartValue);
	hw->setXSignalValue(mpSigMan->mXVal);

	mpSigMan->mYVal = mpSigMan->mpYSig->rampToValue(mpSigMan->mpYSig->mStartValue);
	mpSigMan->mYSigRamp = mpSigMan->mpYSig->goalValueReached(mpSigMan->mpYSig->mStartValue);
	hw->setYSignalValue(mpSigMan->mYVal);

	if(mpSigMan->mXSigRamp  && mpSigMan->mYSigRamp){
		mpSigMan->miDacSignal = 0;
		htim13.Instance->CNT = 0;
		actState = State::PLAY;
	}
}

/**
 * @n Name: doStateAction
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This is the doStateAction method of the PlaySignalState. In this state the output signal is created and the trigger
 * signal is controlled to be on time. (The phase shift of the filter is also accounted for.)
 * There are two options when a singal can be finished:
 * 1. ScanMode = SingleScan --> Signal is finished after one period of the SlowAxis signal.
 * 2. ScanMode = ContinuousMode --> RunMode must be changed to Idle with the Stop Command.
 * After the signal is finished the state changes to RampDownState.
 */
void PlaySignalState::doStateAction(State& actState, shared_ptr<SignalManager> mpSigMan, shared_ptr<Hardware> hw){
	if(mpSigMan->mXVal >= -1.5 && mpSigMan->mXVal <= 1.5 && mpSigMan->mYVal >= -1.5 && mpSigMan->mYVal <= 1.5)
	{
		hw->setXSignalValue(mpSigMan->mXVal);
		hw->setYSignalValue(mpSigMan->mYVal);
	}
	else
	{
		//actState = State::RAMP_DOWN;
	}

	if(mpSigMan->miDacSignal == mpSigMan->mpFastAxis->getFilterPhase()){
		mpSigMan->mpAScanTriggerPico->startTriggerSignal(mpSigMan->mTrigStopCount);
		mpSigMan->mpAScanTriggerBNC->startTriggerSignal(mpSigMan->mTrigStopCount);
	}
	mpSigMan->mXVal = mpSigMan->mpXSig->calcNextValue(mpSigMan->miDacSignal);
	mpSigMan->mYVal = mpSigMan->mpYSig->calcNextValue(mpSigMan->miDacSignal);

	mpSigMan->mXVal = mpSigMan->mpXSig->filterValue(mpSigMan->mXVal);
	mpSigMan->mYVal = mpSigMan->mpYSig->filterValue(mpSigMan->mYVal);

	if(mpSigMan->miDacSignal >= mpSigMan->mStopCount || mpSigMan->mRunningMode == RunningMode::IDLE)
	{
		mpSigMan->miDacSignal = 0;
		if(mpSigMan->mScanningMode != ScanMode::CONTINUOUS_SCAN || mpSigMan->mRunningMode == RunningMode::IDLE)
			actState = State::RAMP_DOWN;
		else
			actState = State::PLAY;
	}
	mpSigMan->miDacSignal++;
}

/**
 * @n Name: doStateAction
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 01.03.18
 *
 * Description:
 * @n This is the doStateAction method of the RampDownState. After the signal is finished this state ensures a smooth
 * transition from the stopping point to the middle point of the Idle State. This ensures a long life to the MEMS-mirror.
 */
void RampDownState::doStateAction(State& actState, shared_ptr<SignalManager> mpSigMan, shared_ptr<Hardware> hw){
	mpSigMan->mXVal = mpSigMan->mpXSig->rampToValue(0.0f);
	mpSigMan->mXSigRamp = mpSigMan->mpXSig->goalValueReached(0.0f);
	hw->setXSignalValue(mpSigMan->mXVal);

	mpSigMan->mYVal = mpSigMan->mpYSig->rampToValue(0.0f);
	mpSigMan->mYSigRamp = mpSigMan->mpYSig->goalValueReached(0.0f);
	hw->setYSignalValue(mpSigMan->mYVal);

	mpSigMan->miDacSignal++;

	if(mpSigMan->mXSigRamp  && mpSigMan->mYSigRamp ){
		mpSigMan->mpAScanTriggerPico->stopTriggerSignal();
		mpSigMan->mpAScanTriggerBNC->stopTriggerSignal();
		actState = State::IDLE;
	}
}
