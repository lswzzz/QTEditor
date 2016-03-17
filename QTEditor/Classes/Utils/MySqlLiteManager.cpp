#include "MySqlLiteManager.h"
#include "qdir.h"
#include "qfile.h"
#include "Global.h"
#include "FontChina.h"

MySqlLiteManager* MySqlLiteManager::instance = nullptr;
QString MySqlLiteManager::dbname = "db";
QSqlDatabase MySqlLiteManager::db;

MySqlLiteManager::MySqlLiteManager()
{
}


MySqlLiteManager::~MySqlLiteManager()
{
}

MySqlLiteManager* MySqlLiteManager::getInstance(){
	if (instance == nullptr){
		instance = new MySqlLiteManager;
		QString path;
		QDir dir;
		path = dir.currentPath();
		bool isCreate = QFile::exists(path + "/" + dbname);
		if (!instance->openDatabase()){
			addConsoleInfo("打开数据库失败");
		}
		if (!isCreate){
			if (!instance->createTable()){
				addConsoleInfo("创建数据表失败");
			}
		}
	}
	return instance;
}

bool MySqlLiteManager::openDatabase()
{
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(dbname);
	if (!db.open()){
		return false;
	}
	return true;
}

bool MySqlLiteManager::createTable()
{
	QSqlQuery query(db);
	query.exec("DROP table dbtable");
	QString tablename = "dbtable";
	bool result = query.exec("create table " + tablename + " (id int primary key, ConfFile varchar(255), dir varchar(255)," + 
		" preShowPath varchar(255), logIndex int, textureCMD varchar(255), imageOptimizeCMD varchar(255), templateDir varchar(255))");
	if (!result)return false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	QString insert_table = "insert into " + tablename + " values(1, '', '" + QDir::currentPath() + "', '" +
		QDir::currentPath() + "/../Castle/Castle.exe" + "', 0, '../TexturePacker/bin/TexturePacker.exe', '../TexturePacker/pngquant.exe', '../Resources/template')";
#else
    QString insert_table = "insert into " + tablename + " values(1, '', '" + QDir::currentPath() + "', '" +
    QDir::currentPath() + "/../Castle.app/Contents/MacOS/Castle" + "', 0, '../TexturePacker.app/Contents/MacOS/TexturePacker', '../TexturePacker/pngquant.exe', '../Resources/template')";
#endif
	if (!query.exec(insert_table)){
		return false;
	}
	return true;
}

QString MySqlLiteManager::getConfigFile()
{
	return baseQuery("ConfFile", "");
}

bool MySqlLiteManager::setConfigFile(QString file)
{
	return baseUpdate("ConfFile", file);
}

QString MySqlLiteManager::getOpenDir()
{
	return baseQuery("dir", "");
}

bool MySqlLiteManager::setOpenDir(QString dir)
{
	return baseUpdate("dir", dir);
}

QString MySqlLiteManager::getPreShowPath()
{
	return baseQuery("preShowPath", "");
}

bool MySqlLiteManager::setPreShowPath(QString path)
{
	return baseUpdate("preShowPath", path);
}

int MySqlLiteManager::getLogIndex()
{
	return baseQuery("logIndex", -1);
}

bool MySqlLiteManager::setLogIndex(int index)
{
	return baseUpdate("logIndex", index);
}

QString MySqlLiteManager::getTemplateDir()
{
	return baseQuery("templateDir", "");
}

QString MySqlLiteManager::getTextureCMD()
{
	return baseQuery("textureCMD", "");
}

QString MySqlLiteManager::getImageOptimizeCMD()
{
	return baseQuery("imageOptimizeCMD", "");
}

bool MySqlLiteManager::baseUpdate(QString field, QString value)
{
	QString update_sql = "update dbtable set " + field + " = :parameter where id = 1";
	QSqlQuery query(db);
	query.prepare(update_sql);
	query.bindValue(":parameter", value);
	return query.exec();
}

bool MySqlLiteManager::baseUpdate(QString field, int value)
{
	QString update_sql = "update dbtable set " + field + " = :parameter where id = 1";
	QSqlQuery query(db);
	query.prepare(update_sql);
	query.bindValue(":parameter", value);
	return query.exec();
}

QString MySqlLiteManager::baseQuery(QString field, QString type_)
{
	QString select_sql = "select " + field + " from dbtable where id = 1";
	QSqlQuery query(db);
	query.exec(select_sql);
	while (query.next()){
		return query.value(field).toString();
	}
	return "";
}

int MySqlLiteManager::baseQuery(QString field, int type_)
{
	QString select_sql = "select " + field + " from dbtable where id = 1";
	QSqlQuery query(db);
	query.exec(select_sql);
	while (query.next()){
		return query.value(field).toInt();
	}
	return -1;
}