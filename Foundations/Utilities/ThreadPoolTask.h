#ifndef _THREADPOOLTASK_H_ 
#define _THREADPOOLTASK_H_ 

class ThreadPoolTask {
private:

public:
	ThreadPoolTask();
	virtual ~ThreadPoolTask();

	bool IsQueued();
	bool IsRunning();
	bool IsFinished();

	bool Schedule();
	bool Cancel();

protected:
	virtual void OnExecute() = 0;
};

#endif
