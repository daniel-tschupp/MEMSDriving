#ifndef BESSEL6THORDER300HZ_H_
#define BESSEL6THORDER300HZ_H_

#include "IFilter.h"

/**
 * \class Bessel6thOrder300Hz
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 30.10.18
 *
 * Description:
 * @n This class implements a bessel filter of the 6th order. To calculate
 * efficiently on the micro controller only float falues are used (FPU).
 * As a Bessel filter of the 6th order calculates with very big numbers it was
 * not possible to implement the filter in one step with floats. So two 3rd
 * order Bessel filter are implemented in serial to achive the 6th order Bessel
 * filter. The difference between the two implementations are minimal.
 *
 */
class Bessel6thOrder300Hz : public IFilter {
private:
	#define ARRAYSIZE 4     // Number of previous values for calculation
	float xv[ARRAYSIZE];    // Array to store previous values 
	float yv[ARRAYSIZE];    // Array to store previous values
	float gain;             // Gain for Bessel calculation       
	float xv2[ARRAYSIZE];   // Array to store previous values
	float yv2[ARRAYSIZE];   // Array to store previous values
	float gain2;            // \brief Gain for Bessel calculation

public:
	
        /** \brief Constructor to create a Bessel6thOrder300Hz object. */
        Bessel6thOrder300Hz(void);

        /** \brief Destructor */
	~Bessel6thOrder300Hz(void);

        /** 
         * \brief Implementation of a digital filter.
         * 
         * @param       nextValue       The next value to be filtered.
         *
         * @return The filtered value.
         */
    	float filter(float nextValue) override;

};


#endif /* FILTER_H_ */
