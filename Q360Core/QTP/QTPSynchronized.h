#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <pthread.h>
//mutex class
class Mutex
{
public:
    //the default constructor
    Mutex()
    {
    	pthread_mutexattr_init( &attr );
    	int val = PTHREAD_MUTEX_RECURSIVE_NP;
    	pthread_mutexattr_settype( &attr, val);
    	pthread_mutex_init(&mutex, &attr);
    }

    //destructor
    ~Mutex()
    {
    	pthread_mutex_destroy(&mutex);
    }

    //lock
    void lock()
    {
    	pthread_mutex_lock(&mutex);
    }

    //unlock
    void unlock()
    {
    	pthread_mutex_unlock(&mutex);
    }

private:
    pthread_mutex_t mutex;
    pthread_mutexattr_t attr;


};

class Lock
{
public:
    //the default constructor
    Lock(Mutex &mutex) : m_mutex(mutex), m_locked(true)
    {
        mutex.lock();
    }

    //the destructor
    ~Lock()
    {
        m_mutex.unlock();
    }

    //report the state of locking when used as a boolean
    operator bool () const
    {
        return m_locked;
    }

    //unlock
    void setUnlock()
    {
        m_locked = false;
    }

private:
    Mutex &m_mutex;
    bool m_locked;
};
#define synchronized(M)  for(Lock M##_lock = M; M##_lock; M##_lock.setUnlock())

#endif /* __MUTEX_H__ */
