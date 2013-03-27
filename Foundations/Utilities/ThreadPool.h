#ifndef _THREADPOOL_H_ 
#define _THREADPOOL_H_ 

#include "pthread.h"
#include "ThreadPoolTask.h"

class ThreadPool {
private:
	static ThreadPool* s_pxInstance;

private:
	ThreadPool();
	~ThreadPool();

	friend class ThreadPoolTask;
	ThreadPool& GetInstance();

public:
	static void Initialize();
	static void Terminate();

	void Schedule(ThreadPoolTask& task);
	void Cancel(ThreadPoolTask& task);
};

#endif
