#pragma once
#include "TaskThread_Queue.h"
#include "Task_Implement.h"
#include "ConcurrentTaskQueue.h"

class Task_TT : public Task_Implement
{
public:
	virtual void run()override;
	virtual bool isSameTask(Task_Implement* task_)override;
	int tag;
};

class Test1
{
public:
	Test1();
	~Test1();
	void test();
};

