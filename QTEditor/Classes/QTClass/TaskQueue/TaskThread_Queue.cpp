#include "TaskThread_Queue.h"


TaskThread_Queue::TaskThread_Queue()
	:_isRunning(false)
	, _isCheck(false)
{
	
}
 
TaskThread_Queue::~TaskThread_Queue()
{
}

std::string TaskThread_Queue::getName()
{
	return queueName;
}

void TaskThread_Queue::setName(std::string name)
{
	queueName = name;
}

void TaskThread_Queue::clearTasks()
{
	//如果mutex被锁定就阻塞
	std::unique_lock<std::recursive_mutex> lk(m_mut);
	if (m_TaskQueue.size() <= 0){
		int size = m_undeleteTasks.size();
		for (size_t i = 0; i < size; i++){
			auto task_ = m_undeleteTasks.front();
			delete task_;
			m_undeleteTasks.pop_front();
		}
	}
}

void TaskThread_Queue::runTask()
{
	std::unique_lock<std::recursive_mutex> lk(m_mut);
	if (_isCheck){
		setUnusedTask();
	}
	Task_Implement* task_ = m_TaskQueue.front();
	std::thread(&Task_Implement::runTask, task_).detach();
}

void TaskThread_Queue::addTask(Task_Implement* task_)
{
	std::unique_lock<std::recursive_mutex> lk(m_mut);
	task_->m_taskQueue = this;
	m_TaskQueue.push_back(task_);
	if (!hasRunTask()){
		runTask();
	}
}

void TaskThread_Queue::runNextTask()
{
	std::unique_lock<std::recursive_mutex> lk(m_mut);
	Task_Implement* task_ = m_TaskQueue.front();
	m_TaskQueue.pop_front();
	m_undeleteTasks.push_back(task_);
	if (m_TaskQueue.size() == 0){
		clearTasks();
	}
	else{
		runTask();
	}
}

bool TaskThread_Queue::hasRunTask()
{
	std::unique_lock<std::recursive_mutex> lk(m_mut);
	if (m_TaskQueue.size() > 0){
		auto task_ = m_TaskQueue.front();
		if (task_->isIdle()){
			return true;
		}
	}
	return false;
}

void TaskThread_Queue::setUnusedTask()
{
	std::unique_lock<std::recursive_mutex> lk(m_mut);
	size_t index = hasRunTask() ? 1 : 0;
	Task_Implement* sTask = m_TaskQueue.at(index);
	while ((index + 1 ) < m_TaskQueue.size() && !sTask->isUsed()){
		sTask = m_TaskQueue.at(++index);
	}
	for (size_t i = index + 1; i < m_TaskQueue.size(); i++){
		auto _task = m_TaskQueue.at(i);
		if (sTask->isSameTask(_task)){
			sTask->setUsed(false);
			sTask = _task;
		}
	}
}

void TaskThread_Queue::setCheck(bool check)
{
	_isCheck = check;
}