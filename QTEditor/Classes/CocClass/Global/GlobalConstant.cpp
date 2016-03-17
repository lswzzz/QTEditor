#include "GlobalConstant.h"
#include "QDir"
#include "QTextStream"

QString g_c_CopyDir = "CopyFile/";
QString g_c_relaResourceDir = "CopyFile/ResourceImage/";
QString g_c_relaExteralDir = "CopyFile/ExteralImage/";
QString g_c_relaFrameDir = "CopyFile/FrameImage/";
QString g_c_relaTemplateDir = "Template/";
QString g_c_rela_ResourceListDir = "CopyFile/ResList/";
QString g_c_rela_ExteralListDir = "CopyFile/ResList/";
QString g_c_rela_FrameListDir = "CopyFile/ResList/";
QString g_c_relaExteralDirRoot = "ExteralImage/";
QString g_c_relaResourceDirRoot = "ResourceImage/";
QString g_c_relaFrameDirRoot = "FrameImage/";
time_t g_curTime;
QString g_c_relaResourceBackupDir = "CopyFile-Backup/ResourceImage/";
QString g_c_relaExteralBackupDir = "CopyFile-Backup/ExteralImage/";
QString g_c_relaFrameBackDir = "CopyFile-Backup/FrameImage/";

bool DeleteDirectory(const QString path, bool deleteRoot)
{
	if (path.isEmpty()){
		return false;
	}
	QDir dir(path);
	if (!dir.exists()){
		return true;
	}
	dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
	QFileInfoList fileList = dir.entryInfoList();
	foreach(QFileInfo fi, fileList){
		if (fi.isFile()){
			fi.dir().remove(fi.fileName());
		}
		else{
			DeleteDirectory(fi.absoluteFilePath(), true);
		}
	}
	if (deleteRoot){
		QString path_ = dir.absolutePath();
		dir.rmdir(path_);
		return true;
	}
	else{
		return false;
	}
}

bool DeleteFileFile(const QString path)
{
	QFile dir(path);
	if (dir.exists()){
		dir.remove(path);
	}
	return true;
}

bool DeleteAllFile(const QString path)
{
	if (path.isEmpty()){
		return false;
	}
	QDir dir(path);
	if (!dir.exists()){
		return true;
	}
	dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
	QFileInfoList fileList = dir.entryInfoList();
	foreach(QFileInfo fi, fileList){
		if (fi.isFile()){
			fi.dir().remove(fi.fileName());
		}
	}
	return true;
}

bool DeleteAllDirectoryNoFile(const QString path)
{
	if (path.isEmpty()){
		return false;
	}
	QDir dir(path);
	if (!dir.exists()){
		return true;
	}
	dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
	QFileInfoList fileList = dir.entryInfoList();
	foreach(QFileInfo fi, fileList){
		if (fi.isFile()){}
		else{
			DeleteDirectory(fi.absoluteFilePath(), true);
		}
	}
	return true;
}

bool copyDirectoryFiles(const QString& fromDir, const QString& toDir, bool coverFileIfExist)
{
	QDir sourceDir(fromDir);
	QDir targetDir(toDir);
	if (!targetDir.exists()){    /**< 如果目标目录不存在，则进行创建 */
		if (!targetDir.mkdir(targetDir.absolutePath()))
			return false;
	}

	QFileInfoList fileInfoList = sourceDir.entryInfoList();
	foreach(QFileInfo fileInfo, fileInfoList){
		if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
			continue;

		if (fileInfo.isDir()){    /**< 当为目录时，递归的进行copy */
			if (!copyDirectoryFiles(fileInfo.filePath(),
				targetDir.filePath(fileInfo.fileName()),
				coverFileIfExist))
				return false;
		}
		else{            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
			if (coverFileIfExist && targetDir.exists(fileInfo.fileName())){
				targetDir.remove(fileInfo.fileName());
			}

			/// 进行文件copy  
			if (!QFile::copy(fileInfo.filePath(),
				targetDir.filePath(fileInfo.fileName()))){
				return false;
			}
		}
	}
	return true;
}

bool copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist)
{
	toDir.replace("\\", "/");
	if (sourceDir == toDir){
		return true;
	}
	if (!QFile::exists(sourceDir)){
		return false;
	}
	QDir *createfile = new QDir;
	bool exist = createfile->exists(toDir);
	if (exist){
		if (coverFileIfExist){
			createfile->remove(toDir);
		}
	}
	QString dir = toDir.section("/", 0, -2);
	exist = createfile->exists(dir);
	if (!exist){
		createANewDir(dir);
	}

	if (!QFile::copy(sourceDir, toDir))
	{
		return false;
	}
	return true;
}

bool createANewDir(QString dir)
{
	QString temp = dir.section("/", 0, 1);
	QString temp2 = dir.section("/", 1);
	int index = 1;
	QDir* createDir = new QDir;
	while (temp2 != ""){
		if (!createDir->exists(temp)){
			createDir->mkdir(temp);
		}
		++index;
		temp = dir.section("/", 0, index);
		temp2 = dir.section("/", index);
		int a = 1;
	}
	return true;
}

QString readFile(QString filename, bool setUTF8)
{
	QFile sFile(filename);
	if (sFile.open(QIODevice::ReadOnly | QIODevice::Text)){
		QTextStream in(&sFile);
		if (setUTF8)
			in.setCodec("UTF-8");
		QString content = in.readAll();
		sFile.close();
		return content;
	}
	return "";
}

bool readFile(QString filename, QJsonDocument& document, bool setUTF8)
{
	QFile sFile(filename);
	if (sFile.open(QIODevice::ReadOnly | QIODevice::Text)){
		QTextStream in(&sFile);
		if (setUTF8)
			in.setCodec("UTF-8");
		QString content = in.readAll();
		sFile.close();
		QJsonParseError jsonError;
		document = QJsonDocument::fromJson(content.toUtf8(), &jsonError);
		if (jsonError.error == QJsonParseError::NoError && document.isObject()){
			return true;
		}
	}
	return false;
}

bool writeFile(QString filename, QString content, bool setUTF8)
{
	QFile tFile(filename);
	if (tFile.open(QIODevice::WriteOnly | QIODevice::Text)){
		QTextStream out(&tFile);
		if (setUTF8)
			out.setCodec("UTF-8");
		out << content;
		out.flush();
	}
	tFile.close();
	return true;
}