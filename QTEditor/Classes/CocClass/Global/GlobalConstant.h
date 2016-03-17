#pragma once
#include "QString"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>

extern QString g_c_CopyDir;
extern QString g_c_relaResourceDir;
extern QString g_c_relaExteralDir;
extern QString g_c_relaFrameDir;
extern QString g_c_relaTemplateDir;
extern QString g_c_rela_ResourceListDir;
extern QString g_c_rela_ExteralListDir;
extern QString g_c_rela_FrameListDir;
extern QString g_c_relaExteralDirRoot;
extern QString g_c_relaResourceDirRoot;
extern QString g_c_relaFrameDirRoot;
extern time_t g_curTime;
extern QString g_c_relaResourceBackupDir;
extern QString g_c_relaExteralBackupDir;
extern QString g_c_relaFrameBackDir;

bool DeleteDirectory(const QString path, bool deleteRoot = false);
bool DeleteFileFile(const QString path);
bool DeleteAllFile(const QString path);
bool DeleteAllDirectoryNoFile(const QString path);
bool copyDirectoryFiles(const QString& fromDir, const QString& toDir, bool coverFileIfExist = true);
bool copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist = true);
bool createANewDir(QString dir);
QString readFile(QString filename, bool setUTF8);
bool	readFile(QString filename, QJsonDocument& document, bool setUTF8);
bool writeFile(QString filename, QString content, bool setUTF8);
