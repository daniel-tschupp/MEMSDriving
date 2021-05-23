#ifndef __GLOBDEF__
#define __GLOBDEF__
extern "C" {
	#include "dac.h"
	#include "math.h"
}
#include "Signal.h"

#define ABS(x)	((x)<0 ? -(x) : (x))

enum SignalStates {	SignalStates_START_SIG,
			SignalStates_RAMP_UP,
			SignalStates_PLAY_SIG, 
			SignalStates_RAMP_DOWN , 
			SignalStates_STOP_SIG,
			SignalStates_IDLE		};
	
enum SignalChoises	{	SignalChoises_SIN,
				SignalChoises_LIN,
				SignalChoises_TRIANGLE	};
						

typedef struct{
		enum SignalChoises sigKind;
		int frequency;
		float w;
		float phase;
		float amplitude;
		float offset;
		float timeIncrement;
} SignalSpecs;



#define MYPI		3.1415926536f
#define MYTWOPI	6.2831853072f

//extern SignalSpecs* ySignal;
//extern SignalSpecs* xSignal;
extern unsigned long int StopCount;

//extern uint32_t calcDACValue(SignalSpecs* signal, float actTime);	
extern DAC_HandleTypeDef hdac;
//extern enum SignalStates SigState;

#endif /* __GLOBDEF__ */	
