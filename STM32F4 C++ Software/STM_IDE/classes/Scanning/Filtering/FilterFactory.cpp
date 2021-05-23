/*
 * FilterFactory.cpp
 *
 *  Created on: 08.01.2020
 *      Author: tud1
 */

#include <FilterFactory.h>

FilterFactory::FilterFactory() {
	mFilterMap["NoFilter"] = make_shared<NoFilterGenerator>();
	mFilterMap["Bessel6thOrder"] = make_shared<BesselGenerator>();
}

FilterFactory::~FilterFactory() {}

void FilterFactory::addIIRFilter(string name, std::vector<float> a, std::vector<float> b, float gain){
	if(mFilterMap.count(name) > 0){
		mFilterMap[name].reset(new IIRFilterGenerator(a, b, gain));

	}
	else
		mFilterMap[name] = make_shared<IIRFilterGenerator>(a, b, gain);
}

std::shared_ptr<IFilter> FilterFactory::generateFilter(string filterName){
	return mFilterMap[filterName]->generateFilter();
}

std::shared_ptr<IFilter> NoFilterGenerator::generateFilter(){
	std::shared_ptr<IFilter> filter = make_shared<NoFilter>();
	return filter;
}

std::shared_ptr<IFilter> BesselGenerator::generateFilter(){
	return make_shared<Bessel6thOrder300Hz>();
}

IIRFilterGenerator::IIRFilterGenerator(){}
IIRFilterGenerator::IIRFilterGenerator(std::vector<float> a, std::vector<float> b, float gain){
	mA = a;
	mB = b;
	mGain = gain;
}

std::shared_ptr<IFilter> IIRFilterGenerator::generateFilter(){
	return make_shared<IIRTransposedDirect>(mB, mA, mGain);
}
