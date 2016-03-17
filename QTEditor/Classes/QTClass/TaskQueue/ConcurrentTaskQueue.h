#pragma once
#include "TaskThread_Queue.h"
#include "ThreadConst.h"
#include "cocos2d.h"

class ConcurrentTaskQueue
{
public:
	static ConcurrentTaskQueue* getInstance();

	bool createNewQueue(std::string tag);
	TaskThread_Queue* getTaskQueue(std::string tag);
	void clearTaskQueue();

private:
	void addTaskQueue(std::string tag, TaskThread_Queue* taskQueue);

	static ConcurrentTaskQueue* instance;
	ConcurrentTaskQueue();
	~ConcurrentTaskQueue();
	std::map<std::string, TaskThread_Queue*> queueMap;
	std::mutex m_mut;
};

