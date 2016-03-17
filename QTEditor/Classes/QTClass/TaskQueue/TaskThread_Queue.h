#pragma once
#include "Task_Implement.h"
#include "cocos2d.h"

using namespace std;

class TaskThread_Queue
{
public:
	TaskThread_Queue();
	~TaskThread_Queue();
	std::string getName();
	void setName(std::string name);
	
	void addTask(Task_Implement* task_);
	void runTask();

	bool isCheck(){ return _isCheck; };
	void setCheck(bool check);
	bool hasRunTask();
private:
	void clearTasks();
	void runNextTask();
	void setUnusedTask();
private:
	
	std::deque<Task_Implement*>  m_TaskQueue;
	std::deque<Task_Implement*>  m_undeleteTasks;
	std::recursive_mutex m_mut;
	bool _isCheck;
	bool _isRunning;
	std::string queueName;
	friend class Task_Implement;
	friend class ConcurrentTaskQueue;
};

