#include "ConcurrentTaskQueue.h"

ConcurrentTaskQueue* ConcurrentTaskQueue::instance = nullptr;

ConcurrentTaskQueue::ConcurrentTaskQueue()
{
}


ConcurrentTaskQueue::~ConcurrentTaskQueue()
{
}

ConcurrentTaskQueue* ConcurrentTaskQueue::getInstance()
{
	if (instance == nullptr){
		instance = new (std::nothrow)ConcurrentTaskQueue;
	}
	return instance;
}

bool ConcurrentTaskQueue::createNewQueue(std::string tag)
{
	if (queueMap[tag] == nullptr){
		auto taskQueue = new (std::nothrow)TaskThread_Queue;
		taskQueue->setName(tag);
		addTaskQueue(tag, taskQueue);
		return true;
	}
	return false;
}

TaskThread_Queue* ConcurrentTaskQueue::getTaskQueue(std::string tag)
{
	if (queueMap[tag] == nullptr){
		createNewQueue(tag);
	}
	return queueMap[tag];
}

void ConcurrentTaskQueue::addTaskQueue(std::string tag, TaskThread_Queue* taskQueue)
{
	std::unique_lock<std::mutex> lk(m_mut);
	//queueMap.insert(std::pair<std::string, TaskThread_Queue*>(tag, taskQueue));
	queueMap[tag] = taskQueue;
}

void ConcurrentTaskQueue::clearTaskQueue()
{
	std::unique_lock<std::mutex> lk(m_mut);
	std::map<std::string, TaskThread_Queue*>::iterator iter;
	for (iter = queueMap.begin(); iter != queueMap.end(); iter++){
		auto taskQueue = iter->second;
		if (!taskQueue->hasRunTask())delete taskQueue;
		queueMap.erase(iter->first);
	}
}