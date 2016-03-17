//#include "MyLogger.h"
//#include "FontChina.h"
//#include "Global.h"
//#include "qdir.h"
//
//MyLogger* MyLogger::instance = nullptr;
//
//static int MAX_SIZE = 10240000 * 2;
//
//MyLogger::MyLogger()
//{
//	QString path = QDir::currentPath() + "/../Resources/log/log4j.properties";
//	Log4QTLogger::configure(path);
//	infoLogger = Log4QTLogger::createLogger("info");
//	errorLogger = Log4QTLogger::createLogger("error");
//	warningLogger = Log4QTLogger::createLogger("warning");
//}
//
//void MyLogger::reLoad()
//{
//	QString path = QDir::currentPath() + "/../Resources/log/log4j.properties";
//	infoLogger->closeLogger();
//	errorLogger->closeLogger();
//	warningLogger->closeLogger();
//	Log4QTLogger::configure(path);
//	infoLogger = Log4QTLogger::createLogger("info");
//	errorLogger = Log4QTLogger::createLogger("error");
//	warningLogger = Log4QTLogger::createLogger("warning");
//}
//
//bool MyLogger::checkInfoFileSize()
//{
//	QString path = QDir::currentPath() + "/../Resources/log/logs/info.txt";
//	QFileInfo fileInfo(path);
//	auto size = fileInfo.size();
//	if (size > MAX_SIZE){
//		return false;
//	}
//	return true;
//}
//
//MyLogger::~MyLogger()
//{
//}
//
//void MyLogger::check()
//{
//	if (!checkInfoFileSize()){
//		reLoad();
//	}
//}
//
//MyLogger* MyLogger::getInstance()
//{
//	if (!instance){
//		instance = new MyLogger();
//	}
//	return instance;
//}
//
//void MyLogger::addInfo(QString str)
//{
//	infoLogger->info(FontChina::G2U(str.toLocal8Bit().data()));
//}
//
//void MyLogger::addInfo(string str)
//{
//	infoLogger->info(FontChina::G2U(str.c_str()));
//}
//
//void MyLogger::addInfo(const char* str)
//{
//	infoLogger->info(FontChina::G2U(str));
//}
//
//void MyLogger::addError(QString str)
//{
//	errorLogger->error(FontChina::G2U(str.toLocal8Bit().data()));
//}
//
//void MyLogger::addError(string str)
//{
//	errorLogger->error(FontChina::G2U(str.c_str()));
//}
//
//void MyLogger::addError(const char* str)
//{
//	errorLogger->error(FontChina::G2U(str));
//}
//
//void MyLogger::addWarning(QString str)
//{
//	warningLogger->warn(FontChina::G2U(str.toLocal8Bit().data()));
//}
//
//void MyLogger::addWarning(string str)
//{
//	warningLogger->warn(FontChina::G2U(str.c_str()));
//}
//
//void MyLogger::addWarning(const char* str)
//{
//	warningLogger->warn(FontChina::G2U(str));
//}
//
//void MyLogger::addResError(QString str)
//{
//
//}
//
//void MyLogger::addResError(string str)
//{
//
//}
//
//void MyLogger::addResError(const char* str)
//{
//
//}
#include "MyLogger.h"
#include "FontChina.h"
#include "Global.h"
#include "log4qt/propertyconfigurator.h"
#include "qdir.h"

MyLogger* MyLogger::instance = nullptr;

static int MAX_SIZE = 10240000 * 2;

MyLogger::MyLogger()
{
	QString path = QDir::currentPath() + "/../Resources/log/log4j.properties";
	Log4Qt::PropertyConfigurator::configure(path);
	infoLogger = Logger::logger("info");
	errorLogger = Logger::logger("error");
	warningLogger = Logger::logger("warning");
}

void MyLogger::reLoad()
{
	QString path = QDir::currentPath() + "/../Resources/log/log4j.properties";
	auto list = infoLogger->appenders();
	for (int i = 0; i < list.size(); i++){
		list.at(i)->close();
	}
	list = errorLogger->appenders();
	for (int i = 0; i < list.size(); i++){
		list.at(i)->close();
	}
	list = warningLogger->appenders();
	for (int i = 0; i < list.size(); i++){
		list.at(i)->close();
	}
	Log4Qt::PropertyConfigurator::configure(path);
	infoLogger = Logger::logger("info");
	errorLogger = Logger::logger("error");
	warningLogger = Logger::logger("warning");
}

bool MyLogger::checkInfoFileSize()
{
	QString path = QDir::currentPath() + "/../Resources/log/logs/info.txt";
	QFileInfo fileInfo(path);
	auto size = fileInfo.size();
	if (size > MAX_SIZE){
		return false;
	}
	return true;
}

MyLogger::~MyLogger()
{
}

void MyLogger::check()
{
	if (!checkInfoFileSize()){
		reLoad();
	}
}

MyLogger* MyLogger::getInstance()
{
	if (!instance){
		instance = new MyLogger();
	}
	return instance;
}

void MyLogger::addInfo(QString str)
{
	infoLogger->info(FontChina::G2U(str.toLocal8Bit().data()));
}

void MyLogger::addInfo(string str)
{
	infoLogger->info(FontChina::G2U(str.c_str()));
}

void MyLogger::addInfo(const char* str)
{
	infoLogger->info(FontChina::G2U(str));
}

void MyLogger::addError(QString str)
{
	errorLogger->error(FontChina::G2U(str.toLocal8Bit().data()));
}

void MyLogger::addError(string str)
{
	errorLogger->error(FontChina::G2U(str.c_str()));
}

void MyLogger::addError(const char* str)
{
	errorLogger->error(FontChina::G2U(str));
}

void MyLogger::addWarning(QString str)
{
	warningLogger->warn(FontChina::G2U(str.toLocal8Bit().data()));
}

void MyLogger::addWarning(string str)
{
	warningLogger->warn(FontChina::G2U(str.c_str()));
}

void MyLogger::addWarning(const char* str)
{
	warningLogger->warn(FontChina::G2U(str));
}

void MyLogger::addResError(QString str)
{

}

void MyLogger::addResError(string str)
{

}

void MyLogger::addResError(const char* str)
{

}