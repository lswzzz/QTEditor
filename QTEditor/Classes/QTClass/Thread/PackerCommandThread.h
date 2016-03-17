#pragma once
#include "Task_Implement.h"
#include "QString"
#include "QStringList"

class PackerCommandThread : public Task_Implement
{
public:
	PackerCommandThread();
	~PackerCommandThread();
	virtual void run()override;
	
	virtual QString getTexturePackerCommnad(QStringList fileList, QString name, QString dir, QString format, QString platform, bool rotation);
	virtual bool runProcess();
	QString _command;
	QString _outputDir;
	QString _outputName;
};

