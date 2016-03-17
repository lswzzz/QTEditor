#pragma once
#include "QString"
#include "cocos2d.h"

USING_NS_CC;

class OutputInfo
{
public:
	
	static OutputInfo* getInstance();
	void initData();

	int getOutputCount();
	void addOutputCount();
	int getCurrentOutputCount();
	void addCurrentOutputCount();
	QString getOutputDir();
	void setOutputDir(QString dir);
	QString getOutputName();
	void setOutputName(QString name);
	int getDynamicCount();
	void addDynamicCount();
	int getStaticCount();
	void addStaticCount();
	bool getShowOutputState();
	void setShowOutputState(bool show);
	void setDynamicCount(int count);
	void setStaticCount(int count);

	int outputCount;
	int currentOutputCount;
	QString outputDir;
	QString outputName;
	int dynamicCount;
	int staticCount;
	bool isShowOutput;
private:
	std::mutex outputCountMutex;
	std::mutex currentOutputCountMutex;
	std::mutex outputDirMutex;
	std::mutex outputNameMutex;
	std::mutex dynamicCountMutex;
	std::mutex staticCountMutex;
	static OutputInfo* _instance;
	OutputInfo();
	~OutputInfo();
};

