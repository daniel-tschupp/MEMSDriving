/*
 * FIFOQueueISRsafe.cpp
 *
 *  Created on: 30.11.2018
 *      Author: tud1
 */

#include <FIFOQueueISRsafe.h>

/**
 * @n Name: DigitalAnalogConverter
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 30.11.2018
 *
 * Description:
 * @n Constructor to create a FIFOQueueISRsafe object.
 */
FIFOQueueISRsafe::FIFOQueueISRsafe(uint32_t queueSize):
	miEnqueue(0),
	miDequeue(0),
	mEnqueuedElements(0),
	mQueueSize(queueSize)
{
	mQueue = new uint32_t[queueSize];
}

/**
 * @n Name: DigitalAnalogConverter
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 30.11.2018
 *
 * Description:
 * @n Destructor of the FIFOQueueISRsafe object.
 */
FIFOQueueISRsafe::~FIFOQueueISRsafe() {
	delete mQueue;
}

/**
 * @n Name: enqueue
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 30.11.2018
 *
 * Description:
 * @n Enqueues a integer value.
 */
bool FIFOQueueISRsafe::enqueue(uint32_t task){
	bool success = false;
	__disable_irq();
	if(mEnqueuedElements<mQueueSize)
	{
		success = true;
		mQueue[miEnqueue] = task;
		miEnqueue++;
		//if(miEnqueue>13)
		//	miEnqueue = 0;
		if(miEnqueue >= mQueueSize - 1)
			miEnqueue = 0;
		mEnqueuedElements++;
	}
	__enable_irq();

	return success;
}

/**
 * @n Name: dequeue
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 30.11.2018
 *
 * Description:
 * @n Dequeues a integer value.
 */
uint32_t FIFOQueueISRsafe::dequeue(void){
	uint32_t dequeuedValue = 0;
	__disable_irq();
	if(mEnqueuedElements > 0)
	{

		dequeuedValue = mQueue[miDequeue];
		miDequeue++;
		if(miDequeue >= mQueueSize - 1)
			miDequeue = 0;
		mEnqueuedElements--;
	}
	__enable_irq();
	return dequeuedValue;
}

/**
 * @n Name: getQueueSize
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 30.11.2018
 *
 * Description:
 * @n Returns the buffer size of the queue.
 */
const uint32_t FIFOQueueISRsafe::getQueueSize(void){
	return mQueueSize;
}

/**
 * @n Name: getNumberOfElements
 * @n Programmer: Daniel Tschupp
 * @n EMail: tud1@bfh.ch
 * @n Date: 30.11.2018
 *
 * Description:
 * @n Returns number of elements enqueued at the moment.
 */
const uint32_t FIFOQueueISRsafe::getNumberOfElements(){
	return mEnqueuedElements;
}
