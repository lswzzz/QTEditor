#pragma once
#include <qstring.h>
#include "ImageSprite.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include "QTreeWidget"
#include "ImageTreeWidget.h"
#include "AnimationTreeWidget.h"

class QTInput
{
public:
	QTInput();
	~QTInput();

	bool ReadConfFile(QString fileName);
	void setProperties(ImageSprite* sprite, QJsonObject& obj);
	void addSpriteToTemplate(ImageSprite* sprite);
	QString getMD5(QString templateFile);
	void copyImageArray(QJsonArray& array, ImageTreeWidget* tree, QTreeWidgetItem* item, bool isResourceWidget);
	void copyAnimationArray(QJsonArray& array, AnimationTreeWidget* tree, QTreeWidgetItem* item);
	QString copyTemplate(QJsonObject& obj);
	void readResourceData(QJsonObject& obj);
	void readTilesData(QJsonObject& obj, QSize initSize, int grid_Width, int grid_Height, int row, int col);
	void readLayersData(QJsonObject& obj, bool hasAssortDir, int sprite_count);
	void readLayersFileData(QJsonObject& obj, bool hasAssortDir, int sprite_count);
	void readSimpleLayerData(QJsonObject& obj, bool hasAssortDir, int i);
	bool outputImage;
	QString templateDir;
	QString resourceDir;
	QString exteralDir;
	QString animationDir;
	QString resListDir;
	QString searthPath;
	QString sourceDir;
	QString sourceResourceDir;
	QString sourceExteralDir;
	QString sourceAnimationDir;
	QString sourceCopyDir;
	bool hasRealDir;
	bool unusualConfDir;
	bool hasCopyFileError;
	QHash<QString, QString> md5Map;
};

