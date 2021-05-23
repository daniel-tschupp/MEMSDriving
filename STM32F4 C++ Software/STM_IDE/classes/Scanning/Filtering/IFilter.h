#ifndef IFILTER_H_
#define IFILTER_H_

#include <memory>
#include <map>

/**
 * \class IFilter
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 30.10.18
 *
 * Description:
 * @n This class is an interface for implementing different filter. This allows 
 * to change filters without touching the main code.
 */
class IFilter {
public:
	/** \brief Virtual Destructor of the signal object. This is needed to be able to delete siblings. */
	virtual ~IFilter(void){};

	/** \brief Purely virtual method for interfacing. */
	virtual float filter(float)=0;

};

class NoFilter : public IFilter{
public:
	NoFilter(){}
	float filter(float val){return val;}
};

#endif /* FILTER_H_ */
