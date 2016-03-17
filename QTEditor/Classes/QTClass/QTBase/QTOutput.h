#pragma once
#include <qstring.h>
#include "ByteArray.h"
#include "BaseBlock.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "ImageSprite.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include "ImageTreeWidget.h"
#include "AnimationTreeWidget.h"

using namespace cocos2d;
using namespace rapidjson;
class QTOutput
{
public:
	struct TileData{
		bool tileType;
		bool isWater;
		unsigned char way;
		bool isSlope;
		Point start;
		Point end;
	};

	QTOutput();
	~QTOutput();
	//导出成Binary所使用的方法
	bool OutputFileBinary(QString file);
	bool OutputTexs(ByteArray* byteArray);
	bool OutputLayers(ByteArray* byteArray);
	bool OutputAnims(ByteArray* byteArray);
	ByteArray* OutputGrids();
	bool getGridNum(int &size_Box, int &size_Italic, int &size_Fluid);
	bool getGridData(BaseBlock** Vec_Box, BaseBlock** Vec_Italic, BaseBlock** Vec_Fluid);
	ByteArray* getByteArrayData(BaseBlock** Vec_Box, BaseBlock** Vec_Italic, BaseBlock** Vec_Fluid, int size_Box, int size_Italic, int size_Fluid);
	unsigned int getBoxData(std::vector<TileData>& tilesData, BaseBlock** Vec_Box, int num_Box, std::vector<Point>& noneData, std::vector<Point>& leftData, std::vector<Point>& rightData, std::vector<Point>& upData, std::vector<Point>& downData);
	unsigned int getItalicData(std::vector<TileData>& tilesData, std::vector<std::vector<Point>>& italicData, BaseBlock** Vec_Italic, int num_Italic);
	bool OutputBoxData(ByteArray* byteArray, TileData data, std::vector<Point> noneData, std::vector<Point>& leftData, std::vector<Point>& rightData, std::vector<Point>& upData, std::vector<Point>& downData);
	bool OutputItalicData(ByteArray* byteArray2, TileData data, std::vector<Point> _data);
	void setBoxData(std::vector<TileData>& tilesData, bool& way, std::vector<Point>& directData, BaseBlock* block, unsigned int& numTiles);

	//可选择是否导出资源
	bool OutputJsonConf(QString file, bool outputimage = false);
	void setPropertiesForSprite(QVariantMap& image, ImageSprite* sprite, bool outputimage = false, QString outputDir = "", QString relaDir = "");
	bool resetTemplate(QString sourceFile, QString templateFile, QString dir, bool outputimage = false);
	bool outputPlist();

	void copyResTree(QVariantList& list, bool outputImage, QString outputDir, QString relativeDir, ImageTreeWidget* treeWidget);
	void copyResInItem(QVariantList& list, bool outputImage, QString outputDir, QString relativeDir, QTreeWidgetItem* item, ImageTreeWidget* treeWidget);

	void copyFrameTree(QVariantList& list, bool outputImage, QString outputDir, QString relativeDir, AnimationTreeWidget* treeWidget);
	void copyFrameInItem(QVariantList& list, bool outputImage, QString outputDir, QString relativeDir, QTreeWidgetItem* item, AnimationTreeWidget* treeWidget);

	void writeResourceData(QVariantMap& document, bool outputimage);
	void writeTilesData(QVariantMap& document, bool outputimage);
	void writeAllLayerData(QVariantMap& document, bool outputimage);
	void writeAllLayerFilesData(QVariantMap& document, bool outputimage);
	void writeSimpleLayerData(ImageSpriteLayer* layer, QVariantMap& layerdata, bool outputimage, int i);
	std::vector<ImageSpriteLayer*> getSceneLayerVecStaticMerge();
	static bool inOutputConf;
	static bool inOutputCastleData;
private:
	ByteArray* byteArray;
	std::mutex g_mutex;
	QString resourcePath;
	QString exteralPath;
	QString framePath;
	QString outputRootDir;
	QString outputDir;
	QString searthPath;
};

