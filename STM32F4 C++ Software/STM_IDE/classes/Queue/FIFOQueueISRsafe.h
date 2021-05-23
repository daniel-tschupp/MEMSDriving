#ifndef QUEUE_FIFOQUEUEISRSAFE_H_
#define QUEUE_FIFOQUEUEISRSAFE_H_

extern "C" {
	#include "stm32f4xx_hal.h"
	#include "stm32f4xx.h"
}

class Tasks{
public:
	enum tasks { IDLE, USB_RECEIVE, WIFI_RECEIVE, SIG_START };
};

/**
 * \class FIFOQueueISRsafe
 * @n Programmer: Daniel Tschupp
 * @n E-Mail: tud1@bfh.ch
 * @n Date: 30.11.18
 *
 * Description:
 * @n This class is provides the functionality of a first in first out queue that is completely interrupt safe. It can
 * be used to procceses events created in an interrupt.
 */
class FIFOQueueISRsafe {
	uint32_t miEnqueue;
	uint32_t miDequeue;
	uint32_t mEnqueuedElements;
	uint32_t mQueueSize;
	uint32_t* mQueue;
public:
	/**
	  * \brief This is the constructor for a FIFOQueueISRsafe object.
	  *
	  * @param      queueSize	Size of the queue FIFO-buffer.
	  */
	FIFOQueueISRsafe(uint32_t queueSize);

	/** \brief This is the destructor for a FIFOQueueISRsafe object. */
	virtual ~FIFOQueueISRsafe(void);

	/**
	  * \brief This method enqueues a integer value.
	  *
	  * @param      task	Number for a task. May be defined by an enumeration for example.
	  * @return	Returns true if enqueuding was successful.
	  */
	bool enqueue(uint32_t task);

	/**
	  * \brief This method dequeues a integer value.
	  *
	  * @return	Number for a task. May be defined by an enumeration for example.
	  */
	uint32_t dequeue(void);

	/**
	  * \brief Getter method to read out the size of the queue buffer.
	  *
	  * @return     Size of the queue buffer.
	  */
	const uint32_t getQueueSize(void);

	/**
	  * \brief  Getter method to read out the number of elements actually in the queue.
	  *
	  * @return      Number of elements actually in the queue.
	  */
	const uint32_t getNumberOfElements(void);
};

#endif /* QUEUE_FIFOQUEUEISRSAFE_H_ */
