#ifndef __QTPTask_HH
#define __QTPTask_HH
//
//  Project : ThreadPool
//  File    : TThreadPool.hh
//  Author  : Ronald Kriemann
//  Purpose : class for managing a pool of threads
//

#include <iostream>
#include <list>

#include "QTPMutex.h"

// no specific processor
const int  NO_PROC = -1;

class QTPTask
{
protected:
    // @cond
        
    // number of processor this job was assigned to
    const int  _job_no;
    bool mVolatile;

    // mutex for synchronisation
    QTPMutex     _sync_mutex;
        
    // @endcond
        
public:
	QTPTask(const int n = NO_PROC);

	QTPTask(bool isvolatile);
	virtual ~QTPTask();
    virtual void run ( void * ptr ) = 0;
        
    int  job_no () const;

    void lock   ();

    void unlock ();

    bool on_proc ( const int  p ) const;

	void notify_();
	void wait_();
	bool mIsVolatile();

	static void DestroyQTPTask(QTPTask* QTPTask);
};

#endif  // __TTHREADPOOL_HH
