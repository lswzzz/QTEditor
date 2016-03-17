#include "OutputInfo.h"

OutputInfo* OutputInfo::_instance = nullptr;

OutputInfo::OutputInfo()
	:outputCount(0)
	, currentOutputCount(0)
	, outputDir("")
	, outputName("")
	, dynamicCount(0)
	, staticCount(0)
	, isShowOutput(false)
{
}

OutputInfo::~OutputInfo()
{
}

OutputInfo* OutputInfo::getInstance()
{
	if (_instance == nullptr){
		_instance = new (std::nothrow)OutputInfo;
	}
	return _instance;
}

void OutputInfo::initData()
{
	outputCount = 0;
	currentOutputCount = 0;
	outputDir = "";
	outputName = "";
	dynamicCount = 0;
	staticCount = 0;
	isShowOutput = false;
}

int OutputInfo::getOutputCount()
{
	return outputCount;
}

void OutputInfo::addOutputCount()
{
	std::unique_lock<std::mutex> lk(outputCountMutex);
	++outputCount;
}

int OutputInfo::getCurrentOutputCount()
{
	return currentOutputCount;
}

void OutputInfo::addCurrentOutputCount()
{
	std::unique_lock<std::mutex> lk(currentOutputCountMutex);
	++currentOutputCount;
}

QString OutputInfo::getOutputDir()
{
	return outputDir;
}

void OutputInfo::setOutputDir(QString dir)
{
	std::unique_lock<std::mutex> lk(outputDirMutex);
	outputDir = dir;
}

QString OutputInfo::getOutputName()
{
	return outputName;
}

void OutputInfo::setOutputName(QString name)
{
	std::unique_lock<std::mutex> lk(outputNameMutex);
	outputName = name;
}

int OutputInfo::getDynamicCount()
{
	return dynamicCount;
}

void OutputInfo::addDynamicCount()
{
	std::unique_lock<std::mutex> lk(dynamicCountMutex);
	++dynamicCount;
}

int OutputInfo::getStaticCount()
{
	return staticCount;
}

void OutputInfo::addStaticCount()
{
	std::unique_lock<std::mutex> lk(staticCountMutex);
	++staticCount;
}

bool OutputInfo::getShowOutputState()
{
	return isShowOutput;
}

void OutputInfo::setShowOutputState(bool show)
{
	isShowOutput = show;
}

void OutputInfo::setDynamicCount(int count)
{
	std::unique_lock<std::mutex> lk(dynamicCountMutex);
	dynamicCount = count;
}

void OutputInfo::setStaticCount(int count)
{
	std::unique_lock<std::mutex> lk(staticCountMutex);
	staticCount = count;
}