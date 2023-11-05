#ifdef QPHOTO_THREAD_POOL
#include <pthread.h>
#include "QTP.h"
#include "QTPThreadManager.h"
#ifdef WIN32
#include <Windows.h>
#endif


//#define SINGLE_PROCESS


#define Q_IDX(idx)	((idx) < 0) ? (QTPThreadManager::DECODE_QUQUE_SIZE+(idx)) : ((idx) >= QTPThreadManager::DECODE_QUQUE_SIZE) ? ((idx)&QUEUE_MOD) : (idx) 
#ifdef WIN32
#else
#include <android/log.h>
#ifdef DEBUG_LOG
#  define  D(x...)  __android_log_print(ANDROID_LOG_INFO,"Qmage",x)
#else
#  define  D(...)	do {} while (0)
#endif

#define  E(x...)  __android_log_print(ANDROID_LOG_ERROR,"Qmage",x)
#endif

QmageJob* QmageJob::createQmageJob(void* data) {
	QmageJob* newJob = new QmageJob();
	newJob->mData = data;
	return newJob;
	};

void QmageJob::destoryTQmageJob(QmageJob* job) {
		if(job != NULL)
			delete job;
	};

void QmageJob::run(void *) {
}

//140426 QPHOTO THREADPOOL START
QTPBaseManager::QTPBaseManager(int maxp) {
	mQphotoPool = QTP::createQTP(maxp);
	mIsUseFuture = false;
	mCurrentTaskIndex = 0;
}

int QTPBaseManager::QphotoSetThreadPool(int maxp) {
	if (mQphotoPool == NULL) {
		mQphotoPool = QTP::createQTP(maxp);
		mIsUseFuture = false;
		mCurrentTaskIndex = 0;
	}
	else
		return -1;
	return 0;
}

QTPBaseManager::~QTPBaseManager() {
	if(mQphotoPool != NULL) {
		QTP::destroyQTP(mQphotoPool);
	}
}

void QTPBaseManager::init() {
	setThreadData(mUserData);
}
void QTPBaseManager::prepare() {
	prepareTask(mUserData);
}
void QTPBaseManager::startThreadPool() {
	startThreadPool(mUserData);
}
void QTPBaseManager::future() {
	if (mIsUseFuture) {
		if (mCurrentTaskIndex < 0 || mCurrentTaskIndex >=mTaskStatue.size())
		
		if (mTaskStatue[mCurrentTaskIndex] == TASK_COMPLETE) {
			future(mUserData);
			mCurrentTaskIndex++;
		}
	}
}
//140426 QPHOTO THREADPOOL END
#endif