/*
 * SignalFactory.h
 *
 *  Created on: 26.02.2019
 *      Author: tud1
 */

#ifndef SCANNING_SIGNAL_SIGNALFACTORY_H_
#define SCANNING_SIGNAL_SIGNALFACTORY_H_

#include <memory>
#include <map>
#include <string>
#include <cstring>
#include "SignalManager.h"
#include "SimpleJSONParser.h"
#include "Signal.h"
#include "SinSignal.h"
#include "TriangleSignal.h"
#include "ConstantSignal.h"
#include "IFilter.h"
#include "FilterFactory.h"

class ISignalFactory{
public:
	virtual ~ISignalFactory(){}
	virtual void generateSignal(shared_ptr<SignalManager>& sigMan, SimpleJSONParser& jsParser)=0;
};

typedef map<string, shared_ptr<ISignalFactory>> SigFactoryMap;

class SignalFactory {
	SigFactoryMap mSigFacMap;
public:
	std::shared_ptr<FilterFactory> mpFilterFactory;
	SignalFactory();
	virtual ~SignalFactory();
	void generateSignal(string sigName, shared_ptr<SignalManager>& sigMan, SimpleJSONParser& jsParser);
};


class SineSignalFactory : public ISignalFactory{
	std::shared_ptr<FilterFactory> mpFilterFactory;
public:
	SineSignalFactory(std::shared_ptr<FilterFactory> filterFactory);
	void generateSignal(shared_ptr<SignalManager>& sigMan, SimpleJSONParser& jsParser);
};

class TriangleSignalFactory : public ISignalFactory{
	std::shared_ptr<FilterFactory> mpFilterFactory;
public:
	TriangleSignalFactory(std::shared_ptr<FilterFactory> filterFactory);
	void generateSignal(shared_ptr<SignalManager>& sigMan, SimpleJSONParser& jsParser);
};

class ConstantSignalFactory : public ISignalFactory{
	std::shared_ptr<FilterFactory> mpFilterFactory;
public:
	ConstantSignalFactory(std::shared_ptr<FilterFactory> filterFactory);
	void generateSignal(shared_ptr<SignalManager>& sigMan, SimpleJSONParser& jsParser);
};



#endif /* SCANNING_SIGNAL_SIGNALFACTORY_H_ */
