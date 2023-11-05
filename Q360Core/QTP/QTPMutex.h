#ifndef __QTPMutex_HH
#define __QTPMutex_HH
//
//  Project   : ThreadPool
//  File      : QTPMutex.hh
//  Author    : Ronald Kriemann
//  Purpose   : baseclass for a thread-able class
//

#include <cstdio>
#include "pthread.h"


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
//
// wrapper for pthread_mutex
//
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

class QTPMutex
{
private:
    //! @cond

    // our condition variable
    pthread_cond_t  _cond;

    //! @endcond
protected:
    //! @cond

    // the mutex itself and the mutex-attr
    pthread_mutex_t      _mutex;
    pthread_mutexattr_t  _mutex_attr;

    //! @endcond
    
public:
    /////////////////////////////////////////////////
    //
    // constructor and destructor
    //

    //! construct unlocked mutex
    QTPMutex ();

    //! dtor
    ~QTPMutex ();

    /////////////////////////////////////////////////
    //
    // usual behavior of a mutex
    //

    //! lock mutex
    void  lock    ();

    //! unlock mutex
    void  unlock  ();

    //! return true if mutex is locked and false, otherwise
    bool is_locked ();

    /////////////////////////////////////////////////
    //
    // condition variable related methods
    //

    //! wait for signal to arrive
    void wait      ();

    //! restart one of the threads, waiting on the cond. variable
    void signal    ();

    //! restart all waiting threads
    void broadcast ();
};
#endif  // __QTPMutex_HH
