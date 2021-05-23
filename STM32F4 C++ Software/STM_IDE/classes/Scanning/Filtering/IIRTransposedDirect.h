#ifndef IIRTRANSPOSEDDIRECT_H
#define IIRTRANSPOSEDDIRECT_H

#include <vector>
#include "IFilter.h"

/**
 * \class IIRTransposedDirect
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 18.02.20
 *
 * Description:
 * @n This class implements an IIR filter structure.
 *
 */
class IIRTransposedDirect: public IFilter{
private:
    std::vector<float> mNum; // numerator of iir filter transfer function
    std::vector<float> mDen; // denumerator of iir filter transfer function
    std::vector<float> mZ;   // old values
    unsigned int mOrder;
    float mGain;
public:
    /** \brief Constructor to create a IIRTransposedDirect object. */
    IIRTransposedDirect(std::vector<float> num_coefficients, std::vector<float> den_coefficientss, float gain);

    /** \brief Destructor */
    ~IIRTransposedDirect();

    /**
      * \brief Implementation of a digital filter.
      *
      * @param       signalValue       The next signal value to be filtered.
      *
      * @return The filtered value.
      */
    float filter(float signalValue);
};

#endif // IIRTRANSPOSEDDIRECT_H
