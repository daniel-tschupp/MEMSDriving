#include "TriangleSignal.h"

/**
 * @n Name: TriangleSignal
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 27.11.2018
 *
 * Description:
 * @n Constructor to create a TriangleSignal object and initializes the important members. A bind must still be
 * performed in order to output the waveform.
 */

TriangleSignal::TriangleSignal(float fs, float f, float amp, float phi, float offset, float peakLoc):
	Signal(f, 1/fs, offset),
	mAmp(amp),
	mPhi(phi/180*M_PI),
	mPhiPeriod(2*static_cast<float>(M_PI)),
	mPeakPhi(peakLoc*mPhiPeriod)
{
	mStartValue = this->calcNextValue(0);
}

/**
 * @n Name: calcNextValue
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 27.11.2018
 *
 * Description:
 * @n This method calculates the next point in the of the triangular waveform. The calculation is based on the
 * integration of a rectangular signal. Thats why the actual integration value is stored in the object.
 */
float TriangleSignal::calcNextValue(unsigned long int iTime){
    return this->generateNextValue(iTime*mTimeIncrement);
}

float TriangleSignal::calcNextFromPhase(float phi){
    float actPhi = phi + mPhi;
    float actVal = 0.0f;

    if(actPhi < 0)
        actPhi += mPhiPeriod;
    if(actPhi > mPhiPeriod)
        actPhi -= mPhiPeriod;

    if(actPhi <= mPeakPhi){
        actVal = -mAmp + 2*mAmp*actPhi/mPeakPhi;
    }
    if(actPhi > mPeakPhi){
        actVal = mAmp - 2*mAmp*(actPhi-mPeakPhi)/(mPhiPeriod-mPeakPhi);
    }
    return actVal+mOffset;
}
float TriangleSignal::generateNextValue(float time){
    unsigned long nFullCycles = static_cast<unsigned int>(time * mFrequency);
    time -= nFullCycles/mFrequency;
    float phi = mPhiPeriod*mFrequency*time;
    return this->calcNextFromPhase(phi);
}

