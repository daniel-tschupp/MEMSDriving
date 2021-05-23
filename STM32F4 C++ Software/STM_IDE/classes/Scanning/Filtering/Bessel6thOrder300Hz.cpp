/*
 * Filter.cpp
 *
 *  Created on: 30.10.2018
 *  Author: tud1
 */

#include <Bessel6thOrder300Hz.h>

/**
 * @n Name: Bessel6thOrder300Hz
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 09.11.18
 *
 * Description:
 * @n Constructor to create a Bessel6thOrder300Hz object.
 */
Bessel6thOrder300Hz::Bessel6thOrder300Hz(void):
	gain(4.449775673e+05f),
	gain2(4.449775673e+05f)
{
	for(int i = 0; i < ARRAYSIZE; i++){
		this->xv[i] = 0;
		this->yv[i] = 0;
		this->xv2[i] = 0;
		this->yv2[i] = 0;
	}
}

/**
 * @n Name: ~Bessel6thOrder300Hz
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 09.11.18
 *
 * Description:
 * @n Destructor to cleanly destruct the object after when its anymore.
 */
Bessel6thOrder300Hz::~Bessel6thOrder300Hz(void){}

/**
 * @n Name: filter
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 09.11.19
 *
 * Description:
 * @n This method implements the Bessel 6th order digital filter with a
 */
float Bessel6thOrder300Hz::filter(float nextValue){

	xv[0] = xv[1]; xv[1] = xv[2]; xv[2] = xv[3];
	xv[3] = nextValue / this->gain;
	yv[0] = yv[1]; yv[1] = yv[2]; yv[2] = yv[3];
	yv[3] =   (xv[0] + xv[3]) + 3.0f * (xv[1] + xv[2])
	                     + (  0.9376118463f * yv[0]) + ( -2.8735581964f * yv[1])
	                     + (  2.9359283717f * yv[2]);

	xv2[0] = xv2[1]; xv2[1] = xv2[2]; xv2[2] = xv2[3];
	xv2[3] = yv[3] / this->gain2;
	yv2[0] = yv2[1]; yv2[1] = yv2[2]; yv2[2] = yv2[3];
	yv2[3] =   (xv2[0] + xv2[3]) + 3.0f * (xv2[1] + xv2[2])
	                     + (  0.9376118463f * yv2[0]) + ( -2.8735581964f * yv2[1])
	                     + (  2.9359283717f * yv2[2]);
	return (yv2[3]);
}

