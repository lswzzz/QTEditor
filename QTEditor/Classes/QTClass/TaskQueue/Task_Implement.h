#pragma once

class TaskThread_Queue;
class Task_Implement
{
public:
	Task_Implement();
	virtual ~Task_Implement();
	//子类线程实现的详细内容
	virtual void run();
	virtual void runFailed();
	//如何可以合并就在子类定义这个方法
	virtual bool isSameTask(Task_Implement* task_);
private:
	void runTask();
	//queue调用
	void runBeforce();
	void runAfter();
	bool isIdle();
	bool isUsed();
	void setUsed(bool used);
	bool _idle;
	bool _isused;
	TaskThread_Queue* m_taskQueue;
	friend class TaskThread_Queue;
	friend class ConcurrentTaskQueue;
};

