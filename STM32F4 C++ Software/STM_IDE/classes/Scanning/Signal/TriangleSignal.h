#ifndef TRIANGLE_SIGNAL_H_
#define TRIANGLE_SIGNAL_H_

extern "C" {
	#include "stm32f4xx_hal.h"
	#include "stm32f4xx.h"
	#include "dac.h"
}
#include "global_definitions.h"
#include "Signal.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif
/**
 * \class TriangleSignal
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 27.11.18
 *
 * Description:
 * @n This class implements the methods to generate a triangle signal waveform.
 */

class TriangleSignal : public Signal
{
    float mAmp;
    float mPhi;
    float mPhiPeriod;
    float mPeakPhi;

public:
    TriangleSignal(float fs, float f, float amp, float phi, float offset, float peakLoc);
    float generateNextValue(float time);
    float calcNextValue(unsigned long int iTime) override;
    float calcNextFromPhase(float phi);
};

#endif
