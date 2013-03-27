#include "ThreadPool.h"
#include "IwDebug.h"

ThreadPool* ThreadPool::s_pxInstance = NULL;

void ThreadPool::Initialize() {
	IwAssert(MYAPP, !s_pxInstance);
	s_pxInstance = new ThreadPool();
}

void ThreadPool::Terminate() {
	IwAssert(MYAPP, s_pxInstance);
	delete s_pxInstance;
	s_pxInstance = NULL;
}

ThreadPool::ThreadPool() {
}

ThreadPool::~ThreadPool() {
}

void ThreadPool::Schedule(ThreadPoolTask& task) {
}

void ThreadPool::Cancel(ThreadPoolTask& task) {
}
