#include "Test1.h"
#include "Global.h"

void Task_TT::run()
{
	int sum = 0;
	for (size_t i = 0; i < 10; i++){
		sum += i;
	}
	addConsoleInfo("ssssss%d", tag);
}

bool Task_TT::isSameTask(Task_Implement* task_)
{
	if (tag % 2 == 0){
		return false;
	}
	return true;
}

Test1::Test1()
{
}

Test1::~Test1()
{
}

void Test1::test()
{
	ConcurrentTaskQueue::getInstance()->createNewQueue("Test");
	TaskThread_Queue* queue_ = ConcurrentTaskQueue::getInstance()->getTaskQueue("Test");
	queue_->setCheck(true);
	for (size_t i = 0; i < 10; i++){
		Task_TT* task_ = new Task_TT;
		task_->tag = i + 1;
		queue_->addTask(task_);
	}
	int a = 1;
}