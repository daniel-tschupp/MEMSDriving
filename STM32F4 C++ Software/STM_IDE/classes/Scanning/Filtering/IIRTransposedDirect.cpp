#include "IIRTransposedDirect.h"
#include <fstream>
#include <iostream>

IIRTransposedDirect::IIRTransposedDirect(std::vector<float> num_coefficients, std::vector<float> den_coefficientss, float gain):
    mNum(num_coefficients),
    mDen(den_coefficientss),
	mGain(gain)
{
    if(mNum.size() > mDen.size())
        mOrder = mNum.size();
    else
        mOrder = mDen.size();
    mZ.resize(mOrder-1, 0);
}

IIRTransposedDirect::~IIRTransposedDirect(){}

// a_x : mDen, b_x mNum
float IIRTransposedDirect::filter(float signalValue){
    float sumForward = 0.0f;
    float sumBackward = 0.0f;
    for(unsigned int iStages = 1; iStages < mOrder; ++iStages){
        sumForward += mZ[iStages-1] * mNum[iStages];
        sumBackward += mZ[iStages-1] * mDen[iStages];
    }
    float zNew = (signalValue - sumBackward)/mDen.at(0);

    mZ.insert(mZ.begin(), zNew);
    mZ.pop_back();

    return (zNew * mNum.at(0) + sumForward)*mGain;
}

