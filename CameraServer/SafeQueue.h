/**
* @file SafeQueue.h
*
* @brief Thread-safe queue with possible generic argument for reuse.
*/
#ifndef SAFEQUEUE_H
#define SAFEQUEUE_H

#include <pthread.h>
#include <list>

template<typename T> class tSafeQueue
{
private:

	unsigned int max_size;
	std::list<T> m_queue;
	pthread_mutex_t m_mutex;
	pthread_cond_t m_condv;

public:
/**
* Constructor initialises thread variables and defaults the queue to 10.
*/
tSafeQueue<T>()
{
	this->max_size = 10;
	pthread_mutex_init(&m_mutex, NULL);
	pthread_cond_init(&m_condv, NULL);
}

/**
* Constructor initialises thread variables and sets the queue size to \max_size.
*
* @param max_size The maximum size of the queue.
*/
tSafeQueue<T>(unsigned int max_size)
{
	this->max_size = max_size;
	pthread_mutex_init(&m_mutex, NULL);
	pthread_cond_init(&m_condv, NULL);
}

/**
* Default destructor.
*/
~tSafeQueue()
{
	pthread_mutex_destroy(&m_mutex);
    	pthread_cond_destroy(&m_condv);
}

/**
* Executes a thread safe \push_back with the given \item
*
* @param item An item of type \T to be inserted into the queue.
*/
void queue(T item)
{
	pthread_mutex_lock(&m_mutex);
	m_queue.push_back(item);
	pthread_cond_signal(&m_condv);
	pthread_mutex_unlock(&m_mutex);
}

/**
* Executes a thread safe \front followed by \pop_front. If the queue is empty, the function waits for the condition to be fired.
*/
T unQueue()
{
	pthread_mutex_lock(&m_mutex);
	while(m_queue.size()==0) {
		pthread_cond_wait(&m_condv, &m_mutex);
	}
	T item = m_queue.front();
	m_queue.pop_front();
	pthread_mutex_unlock(&m_mutex);
	return item;
}
};
#endif
