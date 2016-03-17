#pragma once
#include "PackerCommandThread.h"
#include "cocos2d.h"

USING_NS_CC;

class OutputPackerThread : public PackerCommandThread
{
public:
	OutputPackerThread();
	~OutputPackerThread();
	virtual void run() override;

	virtual void generateCommand(QStringList fileList, QString name, QString dir, QString form, int _type);
	virtual void runPacker();
private:
	int _type;
};

