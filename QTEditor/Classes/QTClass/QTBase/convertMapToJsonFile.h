#pragma once
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <QVariant>
#include <QMap>

class convertMapToJsonFile
{
public:
	convertMapToJsonFile();
	~convertMapToJsonFile();
	void setDocument(QVariantMap* document);
	void setFile(FILE* file);
	void setFile(QString filename);
	QByteArray* startConvert();
	QByteArray* convertMap(QByteArray* ba, QVariantMap* map);
	QByteArray* convertList(QByteArray* ba, QVariantList* list);
	void writeToFile();
	void deleteData();
	static void changeFileToUtf8(QString filename);
	QMap<QString, QVariant>* m_map;
	QByteArray* m_array;
	FILE* m_file;
	bool isCloseFILE;
};

