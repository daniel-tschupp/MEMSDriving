/*
 * FilterFactory.h
 *
 *  Created on: 08.01.2020
 *      Author: tud1
 */

#ifndef SCANNING_FILTERING_FILTERFACTORY_H_
#define SCANNING_FILTERING_FILTERFACTORY_H_
#include <map>
#include <memory>
#include <string>
#include <cstring>

#include "IFilter.h"
#include "Bessel6thOrder300Hz.h"
#include "IIRTransposedDirect.h"

using namespace std;

class IFilterGenerator{
public:
	virtual ~IFilterGenerator(){}
	virtual shared_ptr<IFilter> generateFilter() = 0;
};

typedef map<string, shared_ptr<IFilterGenerator>> FilterFactoryMap;

class FilterFactory {
	FilterFactoryMap mFilterMap;
public:
	FilterFactory();
	virtual ~FilterFactory();
	shared_ptr<IFilter> generateFilter(string filterName);
	void addIIRFilter(string name, std::vector<float> a, std::vector<float> b, float gain);
};


class NoFilterGenerator: public IFilterGenerator{
public:
	shared_ptr<IFilter> generateFilter();
};
class BesselGenerator: public IFilterGenerator{
public:
	shared_ptr<IFilter> generateFilter();
};
class IIRFilterGenerator: public IFilterGenerator{
	std::vector<float> mA;
	std::vector<float> mB;
	float mGain;
public:
	IIRFilterGenerator();
	IIRFilterGenerator(std::vector<float> a, std::vector<float> b, float gain);
	shared_ptr<IFilter> generateFilter();
};

#endif /* SCANNING_FILTERING_FILTERFACTORY_H_ */
