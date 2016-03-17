#pragma once
#include "PackerCommandThread.h"
#include "ImageSpriteLayer.h"

class RunTimePackerThread : public PackerCommandThread
{
public:
	RunTimePackerThread();
	~RunTimePackerThread();
	virtual void run() override;
	virtual bool isSameTask(Task_Implement* task_) override;
	virtual void generateCommand(QStringList fileList, QString outputName, QString outputDir, Node* layer, QString format);
	virtual void runPacker();
private:
	unsigned int		tag;
	ImageSpriteLayer*	_layer;
};

