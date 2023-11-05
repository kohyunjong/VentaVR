#include <pthread.h>

#include "QTPTask.h"

QTPTask::QTPTask(const int n) :
		_job_no(n),mVolatile(true){
}

QTPTask::QTPTask(bool isvolatile) :
		_job_no(0),mVolatile(isvolatile){
}

QTPTask::~QTPTask(){};

int  QTPTask::job_no () const { return _job_no; }

void QTPTask::lock   () { _sync_mutex.lock(); }

void QTPTask::unlock () { _sync_mutex.unlock(); }

bool QTPTask::on_proc ( const int  p ) const
{
    return ((p == NO_PROC) || (_job_no == NO_PROC) || (p == _job_no));
}

void QTPTask::notify_() {
	_sync_mutex.lock();
	_sync_mutex.signal();
	_sync_mutex.unlock();
}

void QTPTask::wait_() {
	_sync_mutex.lock();
	_sync_mutex.wait();
	_sync_mutex.unlock();
}

bool QTPTask::mIsVolatile() {
	return mVolatile;
}

void QTPTask::DestroyQTPTask(QTPTask* QTPTask) {
	if(QTPTask != NULL) {
		 QTPTask->_sync_mutex.is_locked();
	}
}

//Qphoto END

