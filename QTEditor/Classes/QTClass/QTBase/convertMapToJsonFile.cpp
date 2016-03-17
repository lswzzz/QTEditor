#include "convertMapToJsonFile.h"
#include "MyLogger.h"
#include "QFile"
#include "QTextStream"

#define MAX_BLOCK_SIZE 102400
#define MAX_BA_SIZE 1024000

convertMapToJsonFile::convertMapToJsonFile()
{
	m_array = new QByteArray();
	isCloseFILE = false;
}

convertMapToJsonFile::~convertMapToJsonFile()
{
	if (m_array)delete m_array;
}

void convertMapToJsonFile::setDocument(QVariantMap* document)
{
	m_map = document;
}

void convertMapToJsonFile::setFile(FILE* file)
{
	m_file = file;
}

void convertMapToJsonFile::setFile(QString filename)
{
	m_file = fopen(filename.toLocal8Bit().data(), "wb");
	isCloseFILE = true;
}

QByteArray* convertMapToJsonFile::startConvert()
{
	m_array->clear();
	convertMap(m_array, m_map);
	writeToFile();
	return m_array;
}

QByteArray* convertMapToJsonFile::convertMap(QByteArray* ba, QVariantMap* map)
{
	MyLogger::getInstance()->addInfo("the convertMapToJsonFile::convertMap be call, the map size is " + QString::number(map->size()));
	if (m_array->size() >= MAX_BA_SIZE){
		writeToFile();
	}
	ba->append("{");
	QMap<QString, QVariant>::iterator it;
	for (it = map->begin(); it != map->end(); ++it) {
		auto key = it.key();
		ba->append("\"" + key + "\":");
		switch (it->type()){
		case QVariant::Bool:
			if (it->toBool()){
				ba->append("true");
			}
			else{
				ba->append("false");
			}
			break;
		case QVariant::Int:
			ba->append(QString::number(it->toInt()));
			break;
		case QVariant::UInt:
			ba->append(QString::number(it->toInt()));
			break;
		case QVariant::Double:
			ba->append(QString::number(it->toDouble()));
			break;
		case 38:
			ba->append(QString::number(it->toFloat()));
			break;
		case QVariant::String:
			ba->append("\"" + it->toString() + "\"");
			break;
		case QVariant::Map:
		{
			auto map__ = it->toMap();
			convertMap(ba, &map__);
		}
			break;
		case QVariant::List:
		{
			auto list__ = it->toList();
			convertList(ba, &list__);
		}
			break;
		default:
		{
			auto type__ = it->type();
			MyLogger::getInstance()->addError("Map UnKnown type :" + QString::number(type__));
			int a = 1;
		}
			break;
		}
		if (it != map->end() - 1){
			ba->append(",");
		}
	}
	ba->append("}");
	return ba;
}

QByteArray* convertMapToJsonFile::convertList(QByteArray* ba, QVariantList* list)
{
	MyLogger::getInstance()->addInfo("the convertMapToJsonFile::convertList be called, the list size is " + QString::number(list->size()));
	if (m_array->size() >= MAX_BA_SIZE){
		writeToFile();
	}
	ba->append("[");
	for (int i = 0; i < list->size(); ++i) {
		auto data_ = list->at(i);
		switch (data_.type()){
		case QVariant::Bool:
			if (data_.toBool()){
				ba->append("true");
			}
			else{
				ba->append("false");
			}
			break;
		case QVariant::Int:
			ba->append(QString::number(data_.toInt()));
			break;
		case QVariant::UInt:
			ba->append(QString::number(data_.toInt()));
			break;
		case QVariant::Double:
			ba->append(QString::number(data_.toDouble()));
			break;
		case 38:
			ba->append(QString::number(data_.toFloat()));
			break;
		case QVariant::String:
			ba->append("\"" + data_.toString() + "\"");
			break;
		case QVariant::Map:
		{
			auto map__ = data_.toMap();
			convertMap(ba, &map__);
		}
			break;
		case QVariant::List:
		{
			auto list_ = data_.toList();
			convertList(ba, &list_);
		}
			break;
		default:
		{
			auto type__ = data_.type();
			MyLogger::getInstance()->addError("List UnKnown type :" + QString::number(type__));
			int a = 1;
		}
			break;
		}
		if (i < list->size() - 1){
			ba->append(",");
		}
	}
	ba->append("]");
	return ba;
}

void convertMapToJsonFile::deleteData()
{
	delete m_array;
	m_array = nullptr;
	if (isCloseFILE){
		fclose(m_file);
		//delete m_file;
	}
}

void convertMapToJsonFile::writeToFile()
{
	MyLogger::getInstance()->addInfo("the convertMapToJsonFile::writeToFile be called, array size is " + QString::number(m_array->size()));
	unsigned int count = m_array->size();
	const char* data__ = (const char*)m_array->data();
	unsigned int persize = MAX_BLOCK_SIZE;
	unsigned int writeIndex = 0;
	while (writeIndex < count){
		if (writeIndex + persize < count){
			fwrite(&data__[writeIndex], persize, 1, m_file);
		}
		else{
			fwrite(&data__[writeIndex], count - writeIndex, 1, m_file);
		}
		writeIndex += persize;
	}
	m_array->clear();
}

void convertMapToJsonFile::changeFileToUtf8(QString filename)
{
	QFile sFile(filename);
	if (sFile.open(QIODevice::ReadOnly | QIODevice::Text)){
		MyLogger::getInstance()->addInfo("convertMapToJsonFile::changeFileToUtf8 be call");
		QTextStream in(&sFile);
		QString content = in.readAll();
		sFile.close();
		QFile tFile(filename);
		if (tFile.open(QIODevice::WriteOnly | QIODevice::Text)){
			QTextStream out(&tFile);
			out.setCodec("UTF-8");
			out << content;
			out.flush();
			tFile.close();
		}
	}
}