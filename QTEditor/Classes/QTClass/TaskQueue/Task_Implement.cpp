#include "Task_Implement.h"
#include "TaskThread_Queue.h"

Task_Implement::Task_Implement()
	:m_taskQueue(nullptr)
	, _idle(false)
	, _isused(true)
{

}

Task_Implement::~Task_Implement()
{

}

void Task_Implement::run()
{
	//继承类自己实现
}

void Task_Implement::runFailed()
{

}

bool Task_Implement::isSameTask(Task_Implement* task_)
{
	return false;
}

void Task_Implement::runTask()
{
	runBeforce();
	if (isUsed()){
		run();
	}
	else{
		runFailed();
	}
	runAfter();
}

void Task_Implement::runBeforce()
{
	_idle = true;
}

bool Task_Implement::isIdle()
{
	return _idle;
}

bool Task_Implement::isUsed()
{
	return _isused;
}

void Task_Implement::setUsed(bool used)
{
	_isused = used;
}

void Task_Implement::runAfter()
{
	m_taskQueue->runNextTask();
}
