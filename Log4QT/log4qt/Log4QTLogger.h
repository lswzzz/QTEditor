#pragma once
#include "log4qt.h"
#include "logger.h"
using namespace Log4Qt;

class __declspec(dllexport) Log4QTLogger
{
public:
	~Log4QTLogger(){};
	static Log4QTLogger*	createLogger(QString info);
	static void				configure(QString path);
	void					closeLogger();
	void					info(const char* info);
	void					error(const char* info);
	void					warn(const char* info);
	void					debug(const char* info);
	void					fatal(const char* info);
private:
	Log4QTLogger(){};
	Logger* m_logger;
};
