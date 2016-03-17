#include "Log4QTLogger.h"
#include "logger.h"
#include "log4qt/consoleappender.h"
#include "log4qt/propertyconfigurator.h"

Log4QTLogger* Log4QTLogger::createLogger(QString info)
{
	Log4QTLogger* ret = new(std::nothrow) Log4QTLogger;
	if (ret){
		ret->m_logger = Log4Qt::Logger::logger(info);
	}
	return nullptr;

}

void Log4QTLogger::configure(QString path)
{
	Log4Qt::PropertyConfigurator::configure(path);
}

void Log4QTLogger::closeLogger()
{
	auto list = m_logger->appenders();
	for (int i = 0; i < list.size(); i++){
		list.at(i)->close();
	}
}
void Log4QTLogger::info(const char* info)
{
	m_logger->info(info);
}
void Log4QTLogger::error(const char* info)
{
	m_logger->error(info);
}
void Log4QTLogger::warn(const char* info)
{
	m_logger->warn(info);
}
void Log4QTLogger::debug(const char* info)
{
	m_logger->debug(info);
}
void Log4QTLogger::fatal(const char* info)
{
	m_logger->fatal(info);
}