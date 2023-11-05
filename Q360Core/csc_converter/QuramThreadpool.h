/**
 * threadpool.h
 *
 *  Created on: Dec 11, 2010
 *      Author: Tomer Heber (heber.tomer@gmail.com).
 */

#ifndef QURAM_THREADPOOL_H_
#define QURAM_THREADPOOL_H_

#define QURAM_THREAD_POOL_QUEUE_SIZE 10000
struct quram_threadpool;

//#include <android/log.h>
//
//#define  LOGD(x...) __android_log_print(ANDROID_LOG_DEBUG,"QURAM_AGIF",x)
//#define	 LOGE(x...) __android_log_print(ANDROID_LOG_ERROR,"QURAM_AGIF",x)
//#define	 LOGI(x...) __android_log_print(ANDROID_LOG_INFO,	"QURAM_AGIF",x)

/**
 * This function creates a newly allocated thread pool.
 *
 * @param num_of_threads The number of worker thread used in this pool.
 * @return On success returns a newly allocated thread pool, on failure NULL is returned.
 */
struct quram_threadpool* quram_threadpool_init(int num_of_threads);

/**
 * This function adds a routine to be exexuted by the threadpool at some future time.
 *
 * @param pool The thread pool structure.
 * @param routine The routine to be executed.
 * @param data The data to be passed to the routine.
 * @param blocking The threadpool might be overloaded if blocking != 0 the operation will block until it is possible to add the routine to the thread pool. If blocking is 0 and the thread pool is overloaded, the call to this function will return immediately.
 *
 * @return 0 on success.
 * @return -1 on failure.
 * @return -2 when the threadpool is overloaded and blocking is set to 0 (non-blocking).
 */
int quram_threadpool_add_task(struct quram_threadpool *pool, void (*routine)(void*), void *data, int blocking);

/**
 * This function stops all the worker threads (stop & exit). And frees all the allocated memory.
 * In case blocking != 0 the call to this function will block until all worker threads have exited.
 *
 * @param pool The thread pool structure.
 * @param blocking If blocking != 0, the call to this function will block until all worker threads are done.
 */
void quram_threadpool_free(struct quram_threadpool *pool, int blocking);

#endif /* QURAM_THREADPOOL_H_ */
