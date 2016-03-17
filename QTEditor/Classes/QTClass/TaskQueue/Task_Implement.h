#pragma once

class TaskThread_Queue;
class Task_Implement
{
public:
	Task_Implement();
	virtual ~Task_Implement();
	//�����߳�ʵ�ֵ���ϸ����
	virtual void run();
	virtual void runFailed();
	//��ο��Ժϲ��������ඨ���������
	virtual bool isSameTask(Task_Implement* task_);
private:
	void runTask();
	//queue����
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

