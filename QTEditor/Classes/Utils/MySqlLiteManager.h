#pragma once
#include "qsql.h"
#include "qsqldatabase.h"
#include "qsqlquery.h"

class MySqlLiteManager
{
public:
	static MySqlLiteManager* getInstance();

	QString getConfigFile();
	bool setConfigFile(QString file);
	QString getOpenDir();
	bool setOpenDir(QString dir);
	QString getPreShowPath();
	bool setPreShowPath(QString path);
	int getLogIndex();
	bool setLogIndex(int index);
	QString getTemplateDir();
	QString getTextureCMD();
	QString getImageOptimizeCMD();
private:
	MySqlLiteManager();
	~MySqlLiteManager();
	bool openDatabase();
	bool createTable();
	bool baseUpdate(QString field, QString value);
	bool baseUpdate(QString field, int value);
	QString baseQuery(QString field, QString type_);
	int baseQuery(QString field, int type_);

	static MySqlLiteManager* instance;
	static QString dbname;
	static QSqlDatabase db;

};

