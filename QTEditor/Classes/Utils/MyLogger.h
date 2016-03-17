//#pragma once
//#include "Log4QTLogger.h"
//#include "cocos2d.h"
//
//
//using namespace std;
//
//class MyLogger
//{
//public:
//	static MyLogger* getInstance();
//
//	void addInfo(QString str);
//	void addInfo(string str);
//	void addInfo(const char* str);
//
//	void addError(QString str);
//	void addError(string str);
//	void addError(const char* str);
//
//	void addWarning(QString str);
//	void addWarning(string str);
//	void addWarning(const char* str);
//
//	void addResError(QString str);
//	void addResError(string str);
//	void addResError(const char* str);
//
//	static MyLogger* instance;
//
//	void reLoad();
//
//	bool checkInfoFileSize();
//	void check();
//protected:
//	MyLogger();
//	~MyLogger();
//private:
//	Log4QTLogger* infoLogger;
//	Log4QTLogger* errorLogger;
//	Log4QTLogger* warningLogger;
//};
//
#pragma once
#include "log4qt.h"
#include "log4qt/consoleappender.h"
#include "log4qt/logger.h"

using namespace Log4Qt;
using namespace std;

class MyLogger
{
public:
	static MyLogger* getInstance();

	void addInfo(QString str);
	void addInfo(string str);
	void addInfo(const char* str);

	void addError(QString str);
	void addError(string str);
	void addError(const char* str);

	void addWarning(QString str);
	void addWarning(string str);
	void addWarning(const char* str);

	void addResError(QString str);
	void addResError(string str);
	void addResError(const char* str);

	static MyLogger* instance;

	void reLoad();

	bool checkInfoFileSize();
	void check();
protected:
	MyLogger();
	~MyLogger();
private:
	Logger* infoLogger;
	Logger* errorLogger;
	Logger* warningLogger;
};

