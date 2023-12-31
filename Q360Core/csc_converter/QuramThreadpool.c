/**
 * quram_threadpool.c
 *
 *  Created on: Dec 11, 2010
 *      Author: Tomer Heber (heber.tomer@gmail.com).
 */

#include "QuramThreadpool.h"

#include <stdlib.h>
#include <stdio.h>

#ifdef LINUX
#include <pthread.h>
#endif
#ifdef _DEBUG
#include "memory_leak_checker.h"
#endif

//#define THREAD_POOL_DEBUG

#ifdef THREAD_POOL_DEBUG
//#define REPORT_ERROR(...) fprintf (stderr,"line %d - ",__LINE__); fprintf (stderr, __VA_ARGS__); fprintf (stderr,"\n")
#include <android/log.h>
#define REPORT_DEBUG(x...) __android_log_print(ANDROID_LOG_DEBUG,"CSC_DEBUG",x)
#define REPORT_ERROR(x...) __android_log_print(ANDROID_LOG_ERROR,"CSC_ERROR",x)
#else
#define REPORT_DEBUG(...)
#define REPORT_ERROR(...)
#endif /* THREAD_POOL_DEBUG */

struct quram_threadpool_task
{
	void (*routine_cb)(void*);

	void *data;
};

struct quram_threadpool_queue
{
	unsigned int head;
	unsigned int tail;

	unsigned int num_of_cells;

	void *cells[QURAM_THREAD_POOL_QUEUE_SIZE];
};

struct quram_threadpool
{
	struct quram_threadpool_queue tasks_queue;
	struct quram_threadpool_queue free_tasks_queue;

	struct quram_threadpool_task tasks[QURAM_THREAD_POOL_QUEUE_SIZE];

	unsigned short num_of_threads;
	volatile unsigned short stop_flag;

#ifdef LINUX
	pthread_t *thr_arr;

	pthread_mutex_t free_tasks_mutex;
	pthread_mutex_t mutex;
	pthread_cond_t free_tasks_cond;
	pthread_cond_t cond;
#endif
};

/**
 * This function inits a quram_threadpool queue.
 *
 * @param queue The queue structure.
 */
static void quram_threadpool_queue_init(struct quram_threadpool_queue *queue)
{
#ifdef LINUX
	int i;

	for (i = 0; i < QURAM_THREAD_POOL_QUEUE_SIZE; i++)
	{
		queue->cells[i] = NULL;
	}

	queue->head = 0;
	queue->tail = 0;
	queue->num_of_cells = 0;
#endif
}

/**
 * This function adds data to the tail of the queue.
 *
 * @param queue The queue structure.
 * @param data The data to be added to the queue.
 * @return 0 on success (the data was added to the queue) else -1 is returned.
 */
static int quram_threadpool_queue_enqueue(struct quram_threadpool_queue *queue, void *data)
{
#ifdef LINUX
	if (queue->num_of_cells == QURAM_THREAD_POOL_QUEUE_SIZE) {
		REPORT_ERROR("The queue is full, unable to add data to it.");
		return -1;
	}

	if (queue->cells[queue->tail] != NULL) {
		REPORT_ERROR("A problem was detected in the queue (expected NULL, but found a different value).");
		return -1;
	}

	queue->cells[queue->tail] = data;

	queue->num_of_cells++;
	queue->tail++;

	if (queue->tail == QURAM_THREAD_POOL_QUEUE_SIZE) {
		queue->tail = 0;
	}
#endif
	return 0;
}

/**
 * This function removes and returns the head data element in the queue.
 *
 * @param queue The queue structure.
 * @return On success a data element is returned, on failure NULL is returned.
 */
static void *quram_threadpool_queue_dequeue(struct quram_threadpool_queue *queue)
{
#ifdef LINUX
	void *data;

	if (queue->num_of_cells == 0) {
			REPORT_ERROR("Tried to dequeue from an empty queue.");
			return NULL;
	}

	data = queue->cells[queue->head];

	queue->cells[queue->head] = NULL;
	queue->num_of_cells--;

	if (queue->num_of_cells == 0) {
		queue->head = 0;
		queue->tail = 0;
	}
	else {
		queue->head++;
		if (queue->head == QURAM_THREAD_POOL_QUEUE_SIZE) {
			queue->head = 0;
		}
	}
	
	return data;
#else
	return NULL;
#endif

}

/**
 * This function checks if a given queue is empty.
 *
 * @param queue The queue structure.
 * @return 1 if the queue is empty, else 0.
 */
static int quram_threadpool_queue_is_empty(struct quram_threadpool_queue *queue)
{
#ifdef LINUX
	if (queue->num_of_cells == 0) {
		return 1;
	}
#endif
	return 0;
}

/**
 * This function queries for the size of the given queue argument.
 *
 * @param queue The queue structure.
 * @return The size of the queue.
 */
static int quram_threadpool_queue_getsize(struct quram_threadpool_queue *queue)
{
	return queue->num_of_cells;
}

/**
 * This function initializes the a quram_threadpool_task structure.
 *
 * @param task The quram_threadpool_task to init.
 */
static void quram_threadpool_task_init(struct quram_threadpool_task *task)
{
#ifdef LINUX
	task->data = NULL;
	task->routine_cb = NULL;
#endif
}

/**
 * This function obtains a queued task from the pool and returns it.
 * If no such task is available the operation blocks.
 *
 * @param pool The thread pool structure.
 * @return A task or NULL on error (or if thread pool should shut down).
 */
static struct quram_threadpool_task* quram_threadpool_task_get_task(struct quram_threadpool *pool)
{
#ifdef LINUX
	struct quram_threadpool_task* task;

	/*
	if (pool->stop_flag) {
		// The pool should shut down return NULL.
		return NULL;
	}
	*/

	/* Obtain a task */
	if (pthread_mutex_lock(&(pool->mutex))) {
		REPORT_DEBUG("pthread_mutex_lock: ");
		return NULL;
	}

	while (quram_threadpool_queue_is_empty(&(pool->tasks_queue)) && !pool->stop_flag) {
		/* Block until a new task arrives. */
		if (pthread_cond_wait(&(pool->cond),&(pool->mutex))) {
			REPORT_DEBUG("pthread_cond_wait: ");
			if (pthread_mutex_unlock(&(pool->mutex))) {
				REPORT_DEBUG("pthread_mutex_unlock: ");
			}

			return NULL;
		}
	}

	/*
	if (pool->stop_flag) {
		// The pool should shut down return NULL.
		if (pthread_mutex_unlock(&(pool->mutex))) {
			REPORT_DEBUG("pthread_mutex_unlock: ");
		}
		return NULL;
	}
	*/

	if ((task = (struct quram_threadpool_task*)quram_threadpool_queue_dequeue(&(pool->tasks_queue))) == NULL) {
		/* Since task is NULL returning task will return NULL as required. */
		REPORT_ERROR("Failed to obtain a task from the jobs queue.");
	}

	if (pthread_mutex_unlock(&(pool->mutex))) {
		REPORT_DEBUG("pthread_mutex_unlock: ");
		return NULL;
	}

	REPORT_DEBUG("return task : 0x%x", task);

	return task;
#else
	return NULL;
#endif
}

/**
 * This is the routine the worker threads do during their life.
 *
 * @param data Contains a pointer to the quram_threadpool structure.
 * @return NULL.
 */
static void *worker_thr_routine(void *data)
{
#ifdef LINUX
	struct quram_threadpool *pool = (struct quram_threadpool*)data;
	struct quram_threadpool_task *task;

	while (1) {
		task = quram_threadpool_task_get_task(pool);
		REPORT_ERROR("task = %d", task);
		if (task == NULL) {
			if (pool->stop_flag) {
				/* Worker thr needs to exit (thread pool was shutdown). */
				break;
			}
			else {
				/* An error has occurred. */
				REPORT_ERROR("Warning an error has occurred when trying to obtain a worker task.");
				REPORT_ERROR("The worker thread has exited.");
				break;
			}
		}

		/* Execute routine (if any). */
		if (task->routine_cb) {
			task->routine_cb(task->data);

			/* Release the task by returning it to the free_task_queue. */
			quram_threadpool_task_init(task);
			if (pthread_mutex_lock(&(pool->free_tasks_mutex))) {
				REPORT_DEBUG("pthread_mutex_lock: ");
				REPORT_ERROR("The worker thread has exited.");
				break;
			}

			if (quram_threadpool_queue_enqueue(&(pool->free_tasks_queue),task)) {
				REPORT_ERROR("Failed to enqueue a task to free tasks queue.");
				if (pthread_mutex_unlock(&(pool->free_tasks_mutex))) {
					REPORT_DEBUG("pthread_mutex_unlock: ");
				}

				REPORT_ERROR("The worker thread has exited.");
				break;
			}

			if (quram_threadpool_queue_getsize(&(pool->free_tasks_queue)) == 1) {
				/* Notify all waiting threads that new tasks can added. */
				if (pthread_cond_broadcast(&(pool->free_tasks_cond))) {
					REPORT_DEBUG("pthread_cond_broadcast: ");
					if (pthread_mutex_unlock(&(pool->free_tasks_mutex))) {
						REPORT_DEBUG("pthread_mutex_unlock: ");
					}

					break;
				}
			}

			if (pthread_mutex_unlock(&(pool->free_tasks_mutex))) {
				REPORT_DEBUG("pthread_mutex_unlock: ");
				REPORT_ERROR("The worker thread has exited.");
				break;
			}
		}
	}
#endif
	return NULL;
}

/**
 * This callback function does the following steps:
 * 1. It raises a flag that notifies the worker threads to stop working.
 * 2. It waits until all worker threads are done with their execution.
 * 3. It frees all the allocated memory of the threadpool struct.
 *
 * @param ptr The pool to stop its worker threads.

 * @return 0.
 */
static void *stop_worker_thr_routines_cb(void *ptr)
{
#ifdef LINUX
	int i;
	struct quram_threadpool *pool = (struct quram_threadpool*)ptr;

	pool->stop_flag = 1;

	/* Wakeup all worker threads (broadcast operation). */
	if (pthread_mutex_lock(&(pool->mutex))) {
		REPORT_DEBUG("pthread_mutex_lock: ");
		REPORT_ERROR("Warning: Memory was not released.");
		REPORT_ERROR("Warning: Some of the worker threads may have failed to exit.");
		return NULL;
	}

	if (pthread_cond_broadcast(&(pool->cond))) {
		REPORT_DEBUG("pthread_cond_broadcast: ");
		REPORT_ERROR("Warning: Memory was not released.");
		REPORT_ERROR("Warning: Some of the worker threads may have failed to exit.");
		return NULL;
	}

	if (pthread_mutex_unlock(&(pool->mutex))) {
		REPORT_DEBUG("pthread_mutex_unlock: ");
		REPORT_ERROR("Warning: Memory was not released.");
		REPORT_ERROR("Warning: Some of the worker threads may have failed to exit.");
		return NULL;
	}

	/* Wait until all worker threads are done. */
	for (i = 0; i < pool->num_of_threads; i++) {
		if (pthread_join(pool->thr_arr[i],NULL)) {
			REPORT_DEBUG("pthread_join: ");
		}
	}

	/* Free all allocated memory. */
	free(pool->thr_arr);
	free(pool);
#endif
	return NULL;
}

struct quram_threadpool* quram_threadpool_init(int num_of_threads)
{
#ifdef LINUX
	struct quram_threadpool *pool;
	int i;
	/* Create the thread pool struct. */
	if ((pool = malloc(sizeof(struct quram_threadpool))) == NULL) {
		REPORT_DEBUG("malloc: ");
		return NULL;
	}
	pool->stop_flag = 0;

	/* Init the mutex and cond vars. */
	if (pthread_mutex_init(&(pool->free_tasks_mutex),NULL)) {
		REPORT_DEBUG("pthread_mutex_init: ");
		free(pool);
		return NULL;
	}
	if (pthread_mutex_init(&(pool->mutex),NULL)) {
		REPORT_DEBUG("pthread_mutex_init: ");
		free(pool);
		return NULL;
	}
	if (pthread_cond_init(&(pool->free_tasks_cond),NULL)) {
		REPORT_DEBUG("pthread_mutex_init: ");
		free(pool);
		return NULL;
	}
	if (pthread_cond_init(&(pool->cond),NULL)) {
		REPORT_DEBUG("pthread_mutex_init: ");
		free(pool);
		return NULL;
	}
	/* Init the jobs queue. */
	quram_threadpool_queue_init(&(pool->tasks_queue));
	/* Init the free tasks queue. */
	quram_threadpool_queue_init(&(pool->free_tasks_queue));
	/* Add all the free tasks to the free tasks queue. */
	for (i = 0; i < QURAM_THREAD_POOL_QUEUE_SIZE; i++) {
		quram_threadpool_task_init((pool->tasks) + i);
		if (quram_threadpool_queue_enqueue(&(pool->free_tasks_queue),(pool->tasks) + i)) {
			REPORT_ERROR("Failed to a task to the free tasks queue during initialization.");
			free(pool);
			return NULL;
		}
	}
	/* Create the thr_arr. */
	if ((pool->thr_arr = malloc(sizeof(pthread_t) * num_of_threads)) == NULL) {
		REPORT_DEBUG("malloc: ");
		free(pool);
		return NULL;
	}
	/* Start the worker threads. */
	for (pool->num_of_threads = 0; pool->num_of_threads < num_of_threads; (pool->num_of_threads)++) {
		if (pthread_create(&(pool->thr_arr[pool->num_of_threads]),NULL,worker_thr_routine,pool)) {
			REPORT_DEBUG("pthread_create:");

			quram_threadpool_free(pool,0);

			return NULL;
		}
	}
	return pool;
#else
	return NULL;
#endif
}

int quram_threadpool_add_task(struct quram_threadpool *pool, void (*routine)(void*), void *data, int blocking)
{
#ifdef LINUX
	struct quram_threadpool_task *task;

	if (pool == NULL) {
		REPORT_ERROR("The threadpool received as argument is NULL.");
		return -1;
	}

	if (pthread_mutex_lock(&(pool->free_tasks_mutex))) {
		REPORT_DEBUG("pthread_mutex_lock: ");
		return -1;
	}

	/* Check if the free task queue is empty. */
	while (quram_threadpool_queue_is_empty(&(pool->free_tasks_queue))) {
		if (!blocking) {
			/* Return immediately if the command is non blocking. */
			if (pthread_mutex_unlock(&(pool->free_tasks_mutex))) {
				REPORT_DEBUG("pthread_mutex_unlock: ");
				return -1;
			}

			return -2;
		}

		/* blocking is set to 1, wait until free_tasks queue has a task to obtain. */
		if (pthread_cond_wait(&(pool->free_tasks_cond),&(pool->free_tasks_mutex))) {
			REPORT_DEBUG("pthread_cond_wait: ");
			if (pthread_mutex_unlock(&(pool->free_tasks_mutex))) {
				REPORT_DEBUG("pthread_mutex_unlock: ");
			}

			return -1;
		}
	}

	/* Obtain an empty task. */
	if ((task = (struct quram_threadpool_task*)quram_threadpool_queue_dequeue(&(pool->free_tasks_queue))) == NULL) {
		REPORT_ERROR("Failed to obtain an empty task from the free tasks queue.");
		if (pthread_mutex_unlock(&(pool->free_tasks_mutex))) {
			REPORT_DEBUG("pthread_mutex_unlock: ");
		}

		return -1;
	}

	if (pthread_mutex_unlock(&(pool->free_tasks_mutex))) {
		REPORT_DEBUG("pthread_mutex_unlock: ");
		return -1;
	}

	task->data = data;
	task->routine_cb = routine;

	/* Add the task, to the tasks queue. */
	if (pthread_mutex_lock(&(pool->mutex))) {
		REPORT_DEBUG("pthread_mutex_lock: ");
		return -1;
	}

	if (quram_threadpool_queue_enqueue(&(pool->tasks_queue),task)) {
		REPORT_ERROR("Failed to add a new task to the tasks queue.");
		if (pthread_mutex_unlock(&(pool->mutex))) {
			REPORT_DEBUG("pthread_mutex_unlock: ");
		}
		return -1;
	}

	if (quram_threadpool_queue_getsize(&(pool->tasks_queue)) == 1) {
		/* Notify all worker threads that there are new jobs. */
		if (pthread_cond_broadcast(&(pool->cond))) {
			REPORT_DEBUG("pthread_cond_broadcast: ");
			if (pthread_mutex_unlock(&(pool->mutex))) {
				REPORT_DEBUG("pthread_mutex_unlock: ");
			}

			return -1;
		}
	}

	if (pthread_mutex_unlock(&(pool->mutex))) {
		REPORT_DEBUG("pthread_mutex_unlock: ");
		return -1;
	}
#endif
	return 1;
}

void quram_threadpool_free(struct quram_threadpool *pool, int blocking)
{
#ifdef LINUX
	pthread_t thr;

	if (blocking) {
		stop_worker_thr_routines_cb(pool);
	}
	else {
		if (pthread_create(&thr,NULL,stop_worker_thr_routines_cb,pool)) {
			REPORT_DEBUG("pthread_create: ");
			REPORT_ERROR("Warning! will not be able to free memory allocation. This will cause a memory leak.");
			pool->stop_flag = 1;
		}
	}
#endif
}
