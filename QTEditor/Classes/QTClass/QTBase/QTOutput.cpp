 #include "QTOutput.h"
#include "Global.h"
#include "HelloWorldScene.h"
#include "BoxBody.h"
#include "BaseBlock.h"
#include "FluidBody.h"
#include "ItalicBody.h"
#include <QTextSTream>
#include "cocos2d.h"
#include "FontChina.h"
#include "TemplateView.h"
#include "ExterImageListWidget.h"
#include <QDir>
#include "LayerManagerWidget.h"
#include "EntityImageSprite.h"
#include "qteditor.h"
#include <QDesktopWidget>
#include "TemplateValueGenerator.h"
#include "MyLogger.h"
#include "MySqlLiteManager.h"
#include "convertMapToJsonFile.h"
#include "GlobalConstant.h"
#include "ImgTransPath.h"
#include "TexturePackerView.h"
#include "AnimationView.h"

bool QTOutput::inOutputConf = false;
bool QTOutput::inOutputCastleData = false;

QTOutput::QTOutput()
{
	byteArray = NULL;
}

QTOutput::~QTOutput()
{
	if (byteArray)
		delete byteArray;
}

bool QTOutput::getGridNum(int &size_Box, int &size_Italic, int &size_Fluid)
{
	HelloWorld* scene = (HelloWorld*)g_scene;
	size_Box = 0;
	size_Italic = 0;
	size_Fluid = 0;
	for (int i = 0; i < scene->row; i++){
		for (int j = 0; j < scene->col; j++){
			if (scene->gridManagerLayer->getBlock(i, j) && scene->gridManagerLayer->getBlock(i, j)->getType() == Type_Box){
				size_Box++;
			}
			else if (scene->gridManagerLayer->getBlock(i, j) && scene->gridManagerLayer->getBlock(i, j)->getType() == Type_Italic){
				size_Italic++;
			}
			else if (scene->gridManagerLayer->getBlock(i, j) && scene->gridManagerLayer->getBlock(i, j)->getType() == Type_Fluid){
				size_Fluid++;
			}
		}
	}
	MyLogger::getInstance()->addInfo("the QTOutput::getGridNum be called, the getGridNum function are ended, the args size_Box is " + QString::number(size_Box) +
		" size_Italic is " + QString::number(size_Italic) + " size_Fluid is " + QString::number(size_Fluid));
	return true;
}

bool QTOutput::getGridData(BaseBlock** Vec_Box, BaseBlock** Vec_Italic, BaseBlock** Vec_Fluid)
{
	HelloWorld* scene = (HelloWorld*)g_scene;
	int num_Box = 0;
	int num_Italic = 0;
	int num_Fluid = 0;
	for (int i = 0; i < scene->row; i++){
		for (int j = 0; j < scene->col; j++){
			if (scene->gridManagerLayer->getBlock(i, j) && scene->gridManagerLayer->getBlock(i, j)->getType() == Type_Box){
				Vec_Box[num_Box++] = scene->gridManagerLayer->getBlock(i, j);
			}
			else if (scene->gridManagerLayer->getBlock(i, j) && scene->gridManagerLayer->getBlock(i, j)->getType() == Type_Italic){
				Vec_Italic[num_Italic++] = scene->gridManagerLayer->getBlock(i, j);
			}
			else if (scene->gridManagerLayer->getBlock(i, j) && scene->gridManagerLayer->getBlock(i, j)->getType() == Type_Fluid){
				Vec_Fluid[num_Fluid] = scene->gridManagerLayer->getBlock(i, j);
			}
		}
	}
	MyLogger::getInstance()->addInfo("the QTOutput::getGridData be called finished, the Vec_Box size is " + QString::number(num_Box) +
		" the Vec_Italic size is " + QString::number(num_Italic) + " the Vec_Fluid size is " + QString::number(num_Fluid));
	return true;
}

void QTOutput::setBoxData(std::vector<TileData>& tilesData, bool& way, std::vector<Point>& directData, BaseBlock* block, unsigned int& numTiles)
{
	HelloWorld* scene = (HelloWorld*)g_scene;
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	auto blocks = dynamic_cast<BoxBody*>(block);
	if (way == false){
		TileData data;
		data.tileType = true;
		data.isWater = false;
		data.way = blocks->getWay();
		data.isSlope = false;
		tilesData.push_back(data);
		way = true;
		numTiles++;
	}
	Point tile;
	if (g_CocosWindowInitSize.height() > 640){
		if (getNewWindowSize().height() > 640){
			tile = Point(block->getPositionX() / scene->gridWidth, (glview->getFrameSize().height - block->getPositionY()) / scene->gridHeight);
		}
		else{
			tile = Point(block->getPositionX() / scene->gridWidth, (g_CocosWindowInitSize.height() - block->getPositionY()) / scene->gridHeight);
		}
	}
	else{
		if (getNewWindowSize().height() > 640){
			tile = Point(block->getPositionX() / scene->gridWidth, (g_CocosWindowInitSize.height() - block->getPositionY()) / scene->gridHeight);
		}
		else{
			tile = Point(block->getPositionX() / scene->gridWidth, (glview->getFrameSize().height - block->getPositionY()) / scene->gridHeight);
		}
	}
	directData.push_back(tile);
}

unsigned int QTOutput::getBoxData(std::vector<TileData>& tilesData, BaseBlock** Vec_Box, int num_Box, std::vector<Point>& noneData, std::vector<Point>& leftData, std::vector<Point>& rightData, std::vector<Point>& upData, std::vector<Point>& downData)
{
	HelloWorld* scene = (HelloWorld*)g_scene;
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	unsigned int numTiles = 0;
	bool addNoneWay = false;
	bool addUpWay = false;
	bool addLeftWay = false;
	bool addRightWay = false;
	bool addDownWay = false;
	for (int i = 0; i < num_Box; i++){
		auto block = dynamic_cast<BoxBody*>(Vec_Box[i]);
		if (block->getWay() == NONE){
			setBoxData(tilesData, addNoneWay, noneData, block, numTiles);
		}
		else if (block->getWay() == UP){
			setBoxData(tilesData, addUpWay, upData, block, numTiles);
		}
		else if (block->getWay() == DOWN){
			setBoxData(tilesData, addDownWay, downData, block, numTiles);
		}
		else if (block->getWay() == LEFT){
			setBoxData(tilesData, addLeftWay, leftData, block, numTiles);
		}
		else if (block->getWay() == RIGHT){
			setBoxData(tilesData, addRightWay, rightData, block, numTiles);
		}
	}
	return numTiles;
}

//注意这里VecItalic会被清空
unsigned int QTOutput::getItalicData(std::vector<TileData>& tilesData, std::vector<std::vector<Point>>& italicData, BaseBlock** Vec_Italic, int num_Italic)
{
	HelloWorld* scene = (HelloWorld*)g_scene;
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	unsigned int numTiles = 0;
	for (int i = 0; i < num_Italic; i++){
		if (Vec_Italic[i] != NULL){
			auto block = dynamic_cast<ItalicBody*>(Vec_Italic[i]);
			TileData data;
			data.tileType = true;
			data.isWater = false;
			data.isSlope = true;
			data.way = 0;
			Point start = Point(block->getStartPos().x, block->getStartPos().y);
			Point end = Point(block->getEndPos().x, block->getEndPos().y);
			data.start = start;
			data.end = end;
			numTiles++;
			tilesData.push_back(data);
			std::vector<Point> _data;
			for (int j = 0; j < num_Italic; j++){
				if (Vec_Italic[j] != NULL){
					auto block_other = dynamic_cast<ItalicBody*>(Vec_Italic[j]);
					if (block->getStartPos() == block_other->getStartPos() && block->getEndPos() == block_other->getEndPos()){
						Point pos = Point(Vec_Italic[j]->getPositionX() / scene->gridWidth, (glview->getFrameSize().height - Vec_Italic[j]->getPositionY()) / scene->gridHeight);
						if (g_CocosWindowInitSize.height() > 640){
							if (getNewWindowSize().height() > 640){
								pos = Point(Vec_Italic[j]->getPositionX() / scene->gridWidth, (glview->getFrameSize().height - Vec_Italic[j]->getPositionY()) / scene->gridHeight);
							}
							else{
								pos = Point(Vec_Italic[j]->getPositionX() / scene->gridWidth, (g_CocosWindowInitSize.height() - Vec_Italic[j]->getPositionY()) / scene->gridHeight);
							}
						}
						else{
							if (getNewWindowSize().height() > 640){
								pos = Point(Vec_Italic[j]->getPositionX() / scene->gridWidth, (g_CocosWindowInitSize.height() - Vec_Italic[j]->getPositionY()) / scene->gridHeight);
							}
							else{
								pos = Point(Vec_Italic[j]->getPositionX() / scene->gridWidth, (glview->getFrameSize().height - Vec_Italic[j]->getPositionY()) / scene->gridHeight);
							}
						}
						_data.push_back(pos);
						Vec_Italic[j] = NULL;
					}
				}
			}
			italicData.push_back(_data);
			Vec_Italic[i] = NULL;
		}
	}
	return numTiles;
}

bool QTOutput::OutputBoxData(ByteArray* byteArray2, TileData data, std::vector<Point> noneData, std::vector<Point>& leftData, std::vector<Point>& rightData, std::vector<Point>& upData, std::vector<Point>& downData)
{
	unsigned char ccc;
	int numPos = 0;
	unsigned int size = 0;
	std::vector<Point> _data;
	if (data.way == NONE)
	{
		_data = noneData;
		size = noneData.size();
	}
	else if(data.way == UP){
		_data = upData;
		size = upData.size();
	}
	else if (data.way == DOWN){
		_data = downData;
		size = downData.size();
	}
	else if (data.way == LEFT){
		_data = leftData;
		size = leftData.size();
	}
	else{
		_data = rightData;
		size = rightData.size();
	}
	if (size == 1) numPos = 0;
	else if (size <= 255) numPos = 1;
	else if (size <= 65535)	numPos = 2;
	else numPos = 3;
	ccc = ((data.tileType << 7) | (data.isWater << 6) | (data.way << 3) | (data.isSlope << 2) | (numPos));
	byteArray2->writeUnsignedChar(ccc);
	switch (numPos){
	case 1:
	{
			  unsigned char num = size;
			  byteArray2->writeUnsignedChar(num);
	}
		break;
	case 2:
	{
			  unsigned short num = size;
			  byteArray2->writeUnsignedShort(num);
	}
		break;
	case 3:
	{
			  unsigned int num = size;
			  byteArray2->writeUnsignedInt(num);
	}
		break;
	}
	
	for (unsigned int k = 0; k < size; k++){
		int x = (int)_data.at(k).x;
		int y = (int)_data.at(k).y;
		byteArray2->writeUnsignedChar((y >> 4) & 0xF);
		byteArray2->writeUnsignedChar(((y & 0xF) << 4) | ((x >> 8) & 0xF));
		byteArray2->writeUnsignedChar(x & 0xFF);
	}
	return true;
}

bool QTOutput::OutputItalicData(ByteArray* byteArray2, TileData data, std::vector<Point> _data)
{
	unsigned char ccc;
	int numPos = 0;
	unsigned int size = 0;
	size = _data.size();
	if (size == 1) numPos = 0;
	else if (size <= 255) numPos = 1;
	else if (size <= 65535)	numPos = 2;
	else numPos = 3;
	ccc = ((data.tileType << 7) | (data.isWater << 6) | (data.way << 3) | (data.isSlope << 2) | (numPos));
	byteArray2->writeUnsignedChar(ccc);
	byteArray2->writeFloat(data.start.x);
	byteArray2->writeFloat(data.start.y);
	byteArray2->writeFloat(data.end.x);
	byteArray2->writeFloat(data.end.y);
	switch (numPos){
	case 1:
	{
			  unsigned char num = size;
			  byteArray2->writeUnsignedChar(num);
	}
		break;
	case 2:
	{
			  unsigned short num = size;
			  byteArray2->writeUnsignedShort(num);
	}
		break;
	case 3:
	{
			  unsigned int num = size;
			  byteArray2->writeUnsignedInt(num);
	}
		break;
	}
	for (unsigned int k = 0; k < size; k++){
		int x = (int)_data.at(k).x;
		int y = (int)_data.at(k).y;
		byteArray2->writeUnsignedChar((y >> 4) & 0xF);
		byteArray2->writeUnsignedChar(((y & 0xF) << 4) | ((x >> 8) & 0xF));
		byteArray2->writeUnsignedChar(x & 0xFF);
	}
	return true;
}

ByteArray* QTOutput::getByteArrayData(BaseBlock** Vec_Box, BaseBlock** Vec_Italic, BaseBlock** Vec_Fluid, int num_Box, int num_Italic, int num_Fluid)
{
	MyLogger::getInstance()->addInfo("the QTOutput::getByteArrayData be called, the args num_Box is " + QString::number(num_Box) +
		" num_Italic is " + QString::number(num_Italic) + " num_Fluid is " + QString::number(num_Fluid));
	HelloWorld* scene = (HelloWorld*)g_scene;
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	
	std::vector<TileData> tilesData;
	std::vector<Point> noneData;
	std::vector<Point> upData;
	std::vector<Point> leftData;
	std::vector<Point> rightData;
	std::vector<Point> downData;
	std::vector<std::vector<Point>> italicData;
	unsigned int numTiles = 0;
	//导出方块的信息
	numTiles += getBoxData(tilesData, Vec_Box, num_Box, noneData, leftData, rightData, upData, downData);
	//导出斜体
	numTiles += getItalicData(tilesData, italicData, Vec_Italic, num_Italic);
	
	//导出
	int tilesize = tilesData.size();
	int italicnum = 0;
	ByteArray* byteArray2 = new ByteArray(false);
	byteArray2->writeUnsignedShort((int)scene->row);
	byteArray2->writeUnsignedShort((int)scene->col);
	byteArray2->writeUnsignedShort((int)scene->gridWidth);
	byteArray2->writeUnsignedShort((int)scene->gridHeight);
	byteArray2->writeUnsignedInt(numTiles);
	for (int i = 0; i < tilesize; i++){
		TileData data = tilesData.at(i);
		if (data.isSlope == false){
			OutputBoxData(byteArray2, data, noneData, leftData, rightData, upData, downData);
		}
		else{
			std::vector<Point> _data;
			_data = italicData.at(italicnum);
			italicnum++;
			OutputItalicData(byteArray2, data, _data);
		}
	}
	return byteArray2;
}

ByteArray* QTOutput::OutputGrids()
{
	MyLogger::getInstance()->addInfo("the QTOutput::OutputGrids be called");
	char str[256];
	int num_Box = 0;
	int num_Italic = 0;
	int num_Fluid = 0;
	BaseBlock** Vec_Box = NULL;
	BaseBlock** Vec_Italic = NULL;
	BaseBlock** Vec_Fluid = NULL;
	//获取到各种类型网格的数量
	getGridNum(num_Box, num_Italic, num_Fluid);
	Vec_Box = new BaseBlock*[num_Box];
	Vec_Italic = new BaseBlock*[num_Italic];
	Vec_Fluid = new BaseBlock*[num_Fluid];
	//获取到网格数据
	getGridData(Vec_Box, Vec_Italic, Vec_Fluid);
	//sprintf(str, "导出的Box数量%d ,导出的Italic数量%d,导出的Fluid数量%d", num_Box, num_Italic, num_Fluid);
	//addConsoleInfo(str);

	ByteArray* byteArray2 = getByteArrayData(Vec_Box, Vec_Italic, Vec_Fluid, num_Box, num_Italic, num_Fluid);
	delete[]Vec_Box;
	delete[]Vec_Italic;
	delete[]Vec_Fluid;
	return byteArray2;
}

bool QTOutput::OutputTexs(ByteArray* byteArray)
{
	MyLogger::getInstance()->addInfo("the QTOutput::OutputTexs be called, the output data dynamic_Count is " + QString::number(g_dynamic_count) +
		" static_Count is " + QString::number(g_static_count));
	HelloWorld* scene = (HelloWorld*)g_scene;
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	byteArray->writeUnsignedChar(g_dynamic_count + g_static_count);
	for (int i = 0; i < g_dynamic_count; i++){
		char str[256];
		memset(str, 0, 256);
		sprintf(str, "%s_dynamic_%d.png", g_output_Name.c_str(), i);
		byteArray->writeString(str);
		memset(str, 0, 256);
		sprintf(str, "%s_dynamic_%d.plist", g_output_Name.c_str(), i);
		byteArray->writeString(str);
	}
	for (int i = 0; i < g_static_count; i++){
		char str[256];
		memset(str, 0, 256);
		sprintf(str, "%s_static_%d_0.png", g_output_Name.c_str(), i);
		byteArray->writeString(str);
		memset(str, 0, 256);
		sprintf(str, "%s_static_%d_0.plist", g_output_Name.c_str(), i);
		byteArray->writeString(str);
	}
	return true;
}

bool QTOutput::OutputAnims(ByteArray* byteArray)
{
	MyLogger::getInstance()->addInfo("the QTOutput::OutputAnims be called");
	HelloWorld* scene = (HelloWorld*)g_scene;
	auto frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	//numAnims (unsigned short);
	std::vector<animateData> vec = scene->getAllAnimateData();
	unsigned short numAnims = vec.size();
	byteArray->writeUnsignedShort(numAnims);
	for (int i = 0; i < numAnims; i++){
		animateData data = vec.at(i);
		//animAttribute (unsigned char) : 
		unsigned char animType = data.animType;
		unsigned char wrapMode = data.wrapMode;
		byteArray->writeUnsignedChar((animType << 6) | (wrapMode));
		//animName (string);
		std::string animName = data.animName;
		byteArray->writeString(animName);
		if (animType == 0){
			//numFrames (unsigned char)
			unsigned char numFrames = data.numFrames;
			byteArray->writeUnsignedChar(numFrames);
			//delay (float)
			float delay = data.delay;
			byteArray->writeFloat(delay);
			std::vector<int> frameVec = data.frameVec;
			for (int j = 0; j < numFrames; j++){
				//frameName (string)
				int index = frameVec.at(j);
				//只能在动态层使用动画
				std::string frameName = "dynamic" + ImgTransPath::getListPngName(index).toStdString();
				byteArray->writeString(frameName);
			}
		}
	}
	return true;
}

std::vector<ImageSpriteLayer*> convertToStdVec(QVector<ImageSpriteLayer*>* vec)
{
	std::vector<ImageSpriteLayer*> stdVec;
	for (int i = 0; i < vec->size(); i++){
		stdVec.push_back(vec->at(i));
		//addConsoleInfo("%s", vec->at(i)->getTagName().c_str());
	}
	//addConsoleInfo("");
	return stdVec;
}

std::vector<ImageSpriteLayer*> QTOutput::getSceneLayerVecStaticMerge()
{
	std::vector<ImageSpriteLayer*> vecOth;
	HelloWorld* scene = (HelloWorld*)g_scene;
	std::vector<ImageSpriteLayer*> vec = scene->getAllImageLayer();
	QVector<QVector<ImageSpriteLayer*>*> staticLayerVec = static_cast<TexturePackerView*>(g_TextureView)->getVecLayer();
	std::vector<bool> static_index_vec;
	for (int i = 0; i < staticLayerVec.size(); i++){
		static_index_vec.push_back(false);
	}
	for (int i = 0; i < vec.size(); i++){
		auto layer = vec.at(i);
		if (layer->isSportState()){
			if (!layer->isDontOutput()){
				vecOth.push_back(layer);
			}
		}
		else{
			int index = layer->getStaticIndex();
			if (index >= 0){
				if (static_index_vec.at(index) == false){
					vecOth.push_back(layer);
					static_index_vec[index] = true;
				}
			}
		}
	}
	return vecOth;
}

bool QTOutput::OutputLayers(ByteArray* byteArray)
{
	QVector<QVector<ImageSpriteLayer*>*> staticLayerVec = static_cast<TexturePackerView*>(g_TextureView)->getVecLayer();
	std::vector<ImageSpriteLayer*> vec = getSceneLayerVecStaticMerge();
	MyLogger::getInstance()->addInfo("the QTOutput::OutputLayers be called, the output layer count is " + QString::number(vec.size()));
	//numLayers (unsigned char);
	byteArray->writeUnsignedChar(vec.size());
	
	for (int i = 0; i < vec.size(); i++){
		auto layer = vec.at(i);
		
		if (layer->isDontOutput())continue;
		//isStatic (bool);
		unsigned char sta = !layer->isSportState(); 
		unsigned char hasRange = layer->hasRange();
		unsigned layerAttribute = (hasRange << 1) | sta;
		byteArray->writeUnsignedChar(layerAttribute);
		//layerName (string);
		byteArray->writeString(layer->getTagName()); 
		byteArray->writeFloat(layer->getMoveScaleX());
		byteArray->writeFloat(layer->getMoveScaleY());
		byteArray->writeFloat(layer->getSceneScaleFluence());
		byteArray->writeChar(layer->getFilterType());
		if (!layer->isSportState()){
			char str[256];
			memset(str, 0, 256);
			sprintf(str, "%s_static_%d_0.png", g_output_Name.c_str(), layer->getStaticIndex());
			byteArray->writeString(str);
		}
		if (hasRange){
			byteArray->writeUnsignedShort(layer->getRangeLeftTopX());
			byteArray->writeUnsignedShort(layer->getRangeLeftTopY());
			byteArray->writeUnsignedShort(layer->getRangeRightBottomX() - layer->getRangeLeftTopX());
			byteArray->writeUnsignedShort(layer->getRangeRightBottomY() - layer->getRangeLeftTopY());
			int a = 1;
		}
		std::vector<OutputSpriteData*> outputData;
		if (layer->isSportState()){
			outputData = layer->getOutputSpriteData();
		}
		else{
			outputData = layer->getOutputSpriteData(convertToStdVec(staticLayerVec.at(layer->getStaticIndex())));
		}
		//numObjs (unsigned short);
		byteArray->writeUnsignedShort(outputData.size());
		for (int j = 0; j < outputData.size(); j++){
			OutputSpriteData* data = outputData.at(j);
			unsigned char resType = data->resType;
			//bool
			unsigned char isAtlas = data->isAtlas;
			byteArray->writeUnsignedChar((resType << 1) | (isAtlas));
			byteArray->writeUnsignedChar(data->numController);
			{
				if (data->en_Data.hasProperties){
					byteArray->writeUnsignedChar(1);
					entityData en_data = data->en_Data;
					byteArray->writeUnsignedShort(en_data.size());
					int size = en_data.size();
					unsigned char entityAttribute = en_data.entityType;
					byteArray->writeUnsignedChar(entityAttribute);
					byteArray->writeUnsignedInt(en_data.id);
					byteArray->writeFloat(en_data.sleepTime);
					byteArray->writeUnsignedShort(en_data.numGenerate);
					if (en_data.numGenerate != 1){
						byteArray->writeFloat(en_data.generateDelay);
					}
				}
				if (data->monster.hasProperties){
					byteArray->writeUnsignedChar(2);
					monsterData monster = data->monster;
					byteArray->writeUnsignedShort(monster.size());
					int size = monster.size();
					byteArray->writeInt(monster.hp);
					byteArray->writeInt(monster.mp);
					byteArray->writeInt(monster.atk);
					byteArray->writeInt(monster.def);
					byteArray->writeInt(monster.mat);
					byteArray->writeInt(monster.mdf);
					byteArray->writeFloat(monster.spd);
					byteArray->writeFloat(monster.hardFactor);
				}
				if (data->aiData.hasProperties){
					byteArray->writeUnsignedChar(3);
					monsterAIData ai = data->aiData;
					int size = ai.size();
					byteArray->writeUnsignedShort(ai.size());
					byteArray->writeFloat(ai.rangeMinX);
					byteArray->writeFloat(ai.rangeMinY);
					byteArray->writeFloat(ai.rangeMaxX);
					byteArray->writeFloat(ai.rangeMaxY);
					byteArray->writeString(ai.aiFile);
					byteArray->writeUnsignedChar(ai.keyV.size());
					for (int k = 0; k < ai.keyV.size(); k++){
						byteArray->writeString(ai.keyV.at(k)->key);
						byteArray->writeString(ai.keyV.at(k)->value);
					}
				}
				if (data->line_moveData.hasProperties){
					byteArray->writeUnsignedChar(20);
					lineMoveData lineData = data->line_moveData;
					byteArray->writeUnsignedShort(lineData.size());
					int size = lineData.size();
					byteArray->writeFloat(lineData.delay);
					byteArray->writeFloat(lineData.backDelay);
					byteArray->writeFloat(lineData.velocityX);
					byteArray->writeFloat(lineData.velocityY);
					byteArray->writeFloat(lineData.distance); 
					byteArray->writeFloat(lineData.startRunTime);
				}
				if (data->rigidData.hasProperties){
					byteArray->writeUnsignedChar(10);
					rigidAABBData rigidData = data->rigidData;
					byteArray->writeUnsignedShort(rigidData.size());
					int size = rigidData.size();
					byteArray->writeUnsignedChar(rigidData.way);
					byteArray->writeFloat(rigidData.centerX);
					byteArray->writeFloat(rigidData.centerY);
					byteArray->writeFloat(rigidData.halfWidth);
					byteArray->writeFloat(rigidData.halfHeight);
				}
				if (data->triData.hasProperties){
					byteArray->writeUnsignedChar(4);
					triggerData triData = data->triData;
					byteArray->writeUnsignedShort(triData.size());
					int size = triData.size();
					byteArray->writeUnsignedChar(triData.tri_V.size());
					for (int i = 0; i < triData.tri_V.size(); i++){
						byteArray->writeUnsignedChar(triData.tri_V.at(i)->tiggerType);
						byteArray->writeUnsignedInt(triData.tri_V.at(i)->targetID);
					}
				}
				if (data->monsterDrop.hasProperties){
					byteArray->writeUnsignedChar(5);
					monsterDropData mdData = data->monsterDrop;
					byteArray->writeUnsignedShort(mdData.size());
					int size = mdData.size();
					byteArray->writeUnsignedChar(mdData._vec.size());
					for (int i = 0; i < mdData._vec.size(); i++){
						byteArray->writeString(mdData._vec.at(i)->id);
						byteArray->writeFloat(mdData._vec.at(i)->odds);
						byteArray->writeUnsignedChar(mdData._vec.at(i)->difficulty);
					}
				}

				if (data->filData.hasProperties){
					byteArray->writeUnsignedChar(6);
					filterData filData = data->filData;
					byteArray->writeUnsignedShort(filData.size());
					byteArray->writeUnsignedChar(filData.filterType);
				}
			}
			
			if (resType == 0){
				//resName (string) //if isAtlas resName = spriteFrameName, else resName = imgName;
				if (isAtlas){
					if (!layer->isSportState()){
						//std::string str = layer->getTagName() + "_" + data->resName;
						std::string str = data->resName;
						byteArray->writeString(str);
					}
					else{
						byteArray->writeString(data->resName);
					}
					
				}
			}
			else if (resType == 1){
				byteArray->writeString(data->animName);
			}
			else if (resType == 2){
				if (data->playerNumber != ""){
					byteArray->writeString(data->playerNumber);
				}
				else{
					byteArray->writeString(data->resName);
				}
			}
			byteArray->writeUnsignedShort(data->numInstances);
			for (int k = 0; k < data->numInstances; k++){
				Point pos = data->posvec.at(k);
				Vec2 scale = data->scaleVec.at(k);
				float rotation = data->rotationVec.at(k);
				Vec2 anchr = data->anchrVec.at(k);
				Size contentSize = Size(data->contentSize.width*scale.x, data->contentSize.height*scale.y);
				unsigned short x = pos.x + 5000;
				unsigned short y;
				if (g_CocosWindowInitSize.height() > 640){
					y = (g_CocosWindowInitSize.height() - pos.y) + 5000;
				}
				else{
					y = (640 - pos.y) + 5000;
				}
				byteArray->writeUnsignedShort(x);
				byteArray->writeUnsignedShort(y);
				bool hasflipX = data->flipXvec.at(k);
				bool hasflipY = data->flipYvec.at(k);
				bool hasanchrX, hasanchrY, hasscaleX, hasscaleY, hasRotation;
				hasanchrX = false;
				hasanchrY = false;
				hasscaleX = false;
				hasscaleY = false;
				hasRotation = false;
				if (anchr.x != 0.5){
					hasanchrX = true;
				}
				if (anchr.y != 0.5){
					hasanchrY = true;
				}
				if (scale.x != 1.0f){
					hasscaleX = true;
				}
				if (scale.y != 1.0f){
					hasscaleY = true;
				}
				if (rotation != 0.0f){
					hasRotation = true;
				}
				byteArray->writeUnsignedChar(hasRotation << 6 | hasscaleX << 5 | hasscaleY << 4 | hasanchrX << 3 | hasanchrY << 2 | hasflipX << 1 | hasflipY);
				if (hasRotation){
					byteArray->writeFloat(rotation);
				}
				if (hasscaleX){
					byteArray->writeFloat(scale.x);
				}
				if (hasscaleY){
					byteArray->writeFloat(scale.y);
				}
				if (hasanchrX){
					byteArray->writeFloat(anchr.x);
				}
				if (hasanchrY){
					byteArray->writeFloat(anchr.y);
				}
			}
		}
		outputData.clear();
		//使用完要释放掉
		layer->releaseOutputSpriteData();
	}
	return true;
}

bool QTOutput::OutputFileBinary(QString file)
{
	FILE* sFile;
	
	MyLogger::getInstance()->addInfo("the QTOutput::OutputFileBinary be called, the args is " + file);
	sFile = fopen(file.toLocal8Bit().data(), "wb");
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	HelloWorld* scene = (HelloWorld*)g_scene;
	//重置layer对应的layerManager的下表
	//scene->getForeManager()->resetAllLayerIndex();
	Cursor_Type __type = getCursorType();
	setCocCursor(Cursor_Type::Wait);

	//第一步导出配置信息
	//版本号
	if (byteArray) delete byteArray;
	byteArray = new ByteArray(false);
	byteArray->writeUnsignedShort(1);
	//当前已经计算过的格子的数量
	//第二步 导出网格信息
	ByteArray* byteArray2;
	byteArray->writeUnsignedShort(0x0001);
	byteArray2 = OutputGrids();
	byteArray->writeUnsignedInt(byteArray2->getLength());
	byteArray->writeBytes(byteArray2);

	MyLogger::getInstance()->addInfo("the QTOutput::OutputFileBinary output step is 2");
	//第三步 导出纹理信息
	byteArray->writeUnsignedShort(0x0002);
	ByteArray* byteArray3 = new ByteArray(false);
	OutputTexs(byteArray3);
	byteArray->writeUnsignedInt(byteArray3->getLength());
	byteArray->writeBytes(byteArray3);

	MyLogger::getInstance()->addInfo("the QTOutput::OutputFileBinary output step is 3");

	//第四步 导出动画
	ByteArray* byteArray4 = new ByteArray(false);
	if (scene->hasAnimationSprite()){
		byteArray->writeUnsignedShort(0x0003);
		OutputAnims(byteArray4);
		byteArray->writeUnsignedInt(byteArray4->getLength());
		byteArray->writeBytes(byteArray4);
	}

	MyLogger::getInstance()->addInfo("the QTOutput::OutputFileBinary output step is 4");

	//第五步 导出图片层信息
	byteArray->writeUnsignedShort(0x0004);
	ByteArray* byteArray5 = new ByteArray(false);
	OutputLayers(byteArray5);
	byteArray->writeUnsignedInt(byteArray5->getLength());
	byteArray->writeBytes(byteArray5);
	MyLogger::getInstance()->addInfo("the QTOutput::OutputFileBinary output step is 5");

	//导出到文件
	fwrite(byteArray->getBytes(), byteArray->getLength(), 1, sFile);
	fclose(sFile);
	MyLogger::getInstance()->addInfo("the QTOutput::OutputFileBinary successed");
	setCocCursor(__type);
	delete byteArray2;
	delete byteArray3;
	delete byteArray4;
	delete byteArray5;
	
	return true;
}

bool QTOutput::OutputJsonConf(QString file, bool outputimage)
{
	MyLogger::getInstance()->check();
	QTOutput::inOutputConf = true;
	QDir* file_back = new QDir;
	QString back_file = file;
	back_file = back_file.section(".", 0, -2);
	if (back_file.endsWith("-")){
		back_file += "backup.QConf";
	}
	else{
		back_file += "-backup.QConf";
	}
	if (file_back->exists(file)){
		if (file_back->exists(back_file)){
			file_back->remove(back_file);
		}
		QFile::copy(file, back_file);
	}
	FILE* sFile = fopen(file.toUtf8().data(), "wb");
	outputRootDir = file.section("/", 0, -2) + "/";
	outputDir = file.section("/", 0, -2) + "/";
	Cursor_Type __type = getCursorType();
	setCocCursor(Cursor_Type::Wait);
	MyLogger::getInstance()->addInfo("the QTOutput::OutputJsonConf be called, the args is " + file + " " + QString::number(outputimage));
	if (outputimage){
		auto copyPath = outputDir;
		copyPath += g_c_CopyDir;
		DeleteDirectory(outputRootDir + "CopyFile-Backup");
		copyDirectoryFiles(copyPath, outputRootDir + "CopyFile-Backup");
		QDir* dir = new QDir();
		DeleteDirectory(copyPath);
		if (!dir->exists(copyPath)){
			dir->mkdir(copyPath);
		}
		outputDir = copyPath;
		delete dir;
	}
	if (sFile){
		g_mutex.lock();
		HelloWorld* scene = (HelloWorld*)g_scene;
		auto director = Director::getInstance();
		auto glview = director->getOpenGLView();
		QVariantMap document;

		QVariantMap data;
		//第一步导出配置信息
		file = file.section("/", -1);
		document.insert("fileName", FontChina::G2U(file.toUtf8().data()));
		document.insert("width", glview->getFrameSize().width);
		document.insert("height", glview->getFrameSize().height);
		document.insert("initWidth", g_CocosWindowInitSize.width());
		document.insert("initHeight", g_CocosWindowInitSize.height());
		QDesktopWidget *desktop = QApplication::desktop();
		QRect screen = desktop->screenGeometry();
		int screenWidth = screen.width();
		int screenHeight = screen.height();
		document.insert("screenWidth", screenWidth);
		document.insert("screenHeight", screenHeight);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		RECT workRect;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &workRect, 0);
		int iHeight = workRect.bottom - workRect.top;
		int iWidth = workRect.right - workRect.left;
		document.insert("workRectWidth", iWidth);
		document.insert("workRectHeight", iHeight);
		MyLogger::getInstance()->addInfo("QTOutput::OutputJsonConf be called, initWidth is " + QString::number(g_CocosWindowInitSize.width()) +
			" initHeight is " + QString::number(g_CocosWindowInitSize.height()) + " width is " + QString::number(glview->getFrameSize().width) +
			" height is " + QString::number(glview->getFrameSize().height) + " screenWidth is " + QString::number(screenWidth) +
			" screenHeight is " + QString::number(screenHeight) + " workRectWidth is  " + QString::number(iWidth) + " workRectHeight is " +
			QString::number(iHeight));
#else
        QDesktopWidget widget;
        QRect mainScreenSize = widget.availableGeometry(widget.primaryScreen());
        int iHeight = mainScreenSize.width();
        int iWidth = mainScreenSize.height();
        document.insert("workRectWidth", iWidth);
        document.insert("workRectHeight", iHeight);
        MyLogger::getInstance()->addInfo("QTOutput::OutputJsonConf be called, initWidth is " + QString::number(g_CocosWindowInitSize.width()) +
            " initHeight is " + QString::number(g_CocosWindowInitSize.height()) + " width is " + QString::number(glview->getFrameSize().width) +
            " height is " + QString::number(glview->getFrameSize().height) + " screenWidth is " + QString::number(screenWidth) +
            " screenHeight is " + QString::number(screenHeight) + " workRectWidth is  " + QString::number(iWidth) + " workRectHeight is " +
             QString::number(iHeight));

#endif
		document.insert("col", scene->col);
		document.insert("row", scene->row);
		document.insert("isInit", scene->getIsInit());
		document.insert("scenePosx", scene->getPositionX());
		document.insert("scenePosy", scene->getPositionY());
		document.insert("gridWidth", scene->gridWidth);
		document.insert("gridHeight", scene->gridHeight);
		document.insert("scenescale", scene->getScale());
		document.insert("OutputDir", g_output_Dir.c_str());
		document.insert("OutputName", g_output_Name.c_str());
		document.insert("dynamic_count", g_dynamic_count);
		document.insert("static_count", g_static_count);
		document.insert("template_count", g_template_count);
		document.insert("colorR", scene->getLayerColor().r);
		document.insert("colorG", scene->getLayerColor().g);
		document.insert("colorB", scene->getLayerColor().b);
		document.insert("gridColorR", scene->getGridColor().r);
		document.insert("gridColorG", scene->getGridColor().g);
		document.insert("gridColorB", scene->getGridColor().b);
		document.insert("gridColorA", scene->getGridColor().a);
		document.insert("gridVisible", scene->getGridManagerLayer()->isVisible());
		document.insert("foreVisible", scene->getForeManager()->isVisible());
		document.insert("save_Dir", FontChina::G2U(MySqlLiteManager::getInstance()->getOpenDir().toLocal8Bit().data()));
		document.insert("hasAssortDir", true);
		document.insert("hasRealDir", true);
		auto configView = static_cast<ConfigView*>(g_configView);
		bool layerStructure = configView->getLayerStructureState();
		if (!outputimage){
			layerStructure = false;
		}
		document.insert("layerStructure", layerStructure);

		{
			auto layerManager = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
			document.insert("sprite_count", layerManager->getCurrItemIndex());
		}

		if (outputimage){
			document.insert("outputImage", true);
			document.insert("ImageDir", "CopyFile");
		}
		else{
			document.insert("outputImage", false);
		}

		searthPath = QString((FileUtils::getInstance()->getSearchPaths().at(0) + "../Resources/").c_str());
		if (layerStructure){
			document.insert("resourcefile", "resourcefile.img");
			QVariantMap resourceMap;
			writeResourceData(resourceMap, outputimage);
			QString resourceName = outputRootDir + "resourcefile.img";
			auto convert = new convertMapToJsonFile();
			convert->setDocument(&resourceMap);
			convert->setFile(resourceName);
			convert->startConvert();
			convert->deleteData();
			delete convert;
			convertMapToJsonFile::changeFileToUtf8(resourceName);
		}
		else{
			writeResourceData(document, outputimage);
		}

		if (layerStructure){
			document.insert("tilesfile", "tiles.blk");
			QVariantMap tilesMap;
			writeTilesData(tilesMap, outputimage);
			QString tilesName = outputRootDir + "tiles.blk";
			auto convert = new convertMapToJsonFile();
			convert->setDocument(&tilesMap);
			convert->setFile(tilesName);
			convert->startConvert();
			convert->deleteData();
			delete convert;
			convertMapToJsonFile::changeFileToUtf8(tilesName);
		}
		else{
			writeTilesData(data, outputimage);
		}

		if (layerStructure){
			writeAllLayerFilesData(data, outputimage);
		}
		else{
			writeAllLayerData(data, outputimage);
		}

		document.insert("data", data);
		MyLogger::getInstance()->addInfo("start convert to QByteArray");
		auto convert = new convertMapToJsonFile();
		convert->setDocument(&document);
		convert->setFile(sFile);
		MyLogger::getInstance()->addInfo("start write to File " + file);
		convert->startConvert();
		convert->deleteData();
		delete convert;
		fclose(sFile);
		convertMapToJsonFile::changeFileToUtf8(file);
		setCocCursor(__type);
		MyLogger::getInstance()->addInfo("the QTOutput::OutputJsonConf is finished");
		if (!outputimage){
			scene->setAutoSaveLabelVisible(false);
		}
		g_mutex.unlock();
		QTOutput::inOutputConf = false;
		return true;
	}
	QTOutput::inOutputConf = false;
	return false;
}

void QTOutput::setPropertiesForSprite(QVariantMap& image, ImageSprite* sprite, bool outputimage, QString outputDir, QString relaDir)
{
	if(outputimage && sprite->getTemplateFile() != ""){
		QString file = sprite->getTemplateFile().c_str();
		QString templateFile = outputDir + "template/" + file.section("/", -1);
		QString relatFile = relaDir + "template/" + file.section("/", -1);
		image.insert("templateFile", FontChina::G2U(templateFile.toLocal8Bit().data()));
		image.insert("relatFile", FontChina::G2U(relatFile.toLocal8Bit().data()));
	}
	else{
		image.insert("templateFile", FontChina::G2U(sprite->getTemplateFile().c_str()));
	}
	image.insert("templatemd5", sprite->getTemplateMD5().c_str());
	image.insert("start_x", sprite->getStartPos().x);
	image.insert("start_y", sprite->getStartPos().y);
	switch (sprite->getType()){
	case SpriteType::DYNAMIC_BLOCK:
		if (sprite->isHasAnimation()){
			image.insert("Image_Type", 2);
		}
		else{
			image.insert("Image_Type", 0);
		}
		
		break;
	case SpriteType::MONSTER:
		image.insert("Image_Type", 1);
		break;
	case SpriteType::ROLE:
		image.insert("Image_Type", 1);
		break;
	case SpriteType::NORMAL_BLOCK:
		image.insert("Image_Type", 0);
		break;
	}

	//Properties
	//baseProperties
	
	image.insert("baseProperties", TemplateValueGenerator::setBasePropertiesFromSprite(sprite, false));
	image.insert("hasBaseProperties", true);
	  
	if (sprite->isHasMoveProperties()){
		image.insert("moveProperties", TemplateValueGenerator::setMovePropertiesFromSprite(sprite, false));
	}
	image.insert("hasMoveProperties", sprite->isHasMoveProperties());
	if (sprite->isHasBoxProperties()){
		image.insert("boxProperties", TemplateValueGenerator::setBoxPropertiesFromSprite(sprite, false));
		image.insert("hasBoxProperties", true);
		image.insert("hasCircleProperties", false);
	}
	else if (sprite->isHasCircleProperties()){
		image.insert("circleProperties", TemplateValueGenerator::setCirclePropertiesFromSprite(sprite, false));
		image.insert("hasBoxProperties", false);
		image.insert("hasCircleProperties", true);
	}
	else{
		image.insert("hasBoxProperties", false);
		image.insert("hasCircleProperties", false);
	}
	
	if (sprite->isHasEntityProperties()){
		image.insert("entityProperties", TemplateValueGenerator::setEntityPropertiesFromSprite(sprite, false));
	}
	image.insert("hasEntityProperties", sprite->isHasEntityProperties());

	if (sprite->isHasMonsterProperties()){
		image.insert("monsterProperties", TemplateValueGenerator::setMonsterPropertiesFromSprite(sprite, false));
	}
	image.insert("hasMonsterProperties", sprite->isHasMonsterProperties());

	if (sprite->isHasMonsterDropProperties()){
		image.insert("monsterDropProperties", TemplateValueGenerator::setMonsterDropPropertiesFromSprite(sprite, false));
	}
	image.insert("hasMonsterDropProperties", sprite->isHasMonsterDropProperties());

	if (sprite->isHasAIProperties()){
		image.insert("aiProperties", TemplateValueGenerator::setAIPropertiesFromSprite(sprite, false));
	}
	image.insert("hasAIProperties", sprite->isHasAIProperties());

	if (sprite->isHasSimplerTrigger()){
		image.insert("simpleTriggerProperties", TemplateValueGenerator::setSimplerTriggerPropertiesFromSprite(sprite, false));
	}
	image.insert("hasSimpleTriggerProperties", sprite->isHasSimplerTrigger());

	//Action
	if (sprite->isHasAnimation()){
		image.insert("actionProperties", TemplateValueGenerator::setAnimationPropertiesFromSprite(sprite, false, outputimage, relaDir));
	}
	image.insert("hasAnimation", sprite->isHasAnimation());	

	if (sprite->isHasFilterProperties()){
		image.insert("filterProperties", TemplateValueGenerator::setFilterPropertiesFromSprite(sprite, false));
	}
	image.insert("hasFilterProperties", sprite->isHasFilterProperties());
}

bool QTOutput::resetTemplate(QString sourceFile, QString templateFile, QString dir, bool outputimage)
{

	QFile sFile(sourceFile);
	QFile dFile(templateFile);
	if (sFile.open(QIODevice::ReadOnly | QIODevice::Text) && dFile.open(QIODevice::WriteOnly | QIODevice::Text)){
		QTextStream in(&sFile);
		in.setCodec("UTF-8");
		QTextStream out(&dFile);
		QString content = in.readAll();
		sFile.close();
		QJsonParseError jsonError;
		QJsonDocument document = QJsonDocument::fromJson(content.toUtf8(), &jsonError);
		if (jsonError.error == QJsonParseError::NoError && document.isObject()){
			QJsonObject obj = document.object();
			QString relaDir = obj["relaDir"].toString();
			if (outputimage){
				obj["userOutput"] = true;
			}
			else{
				obj["userOutput"] = false;
			}
			QJsonDocument doc = QJsonDocument::fromVariant(obj.toVariantMap());
			out << doc.toJson();
			dFile.close();
			convertMapToJsonFile::changeFileToUtf8(templateFile);
			return true;
		}
	}
	sFile.close();
	dFile.close();
	return false;
}

void QTOutput::copyResTree(QVariantList& list, bool outputImage, QString outputDir, QString relativeDir, ImageTreeWidget* treeWidget)
{
	int size = treeWidget->topLevelItemCount();
	for (int i = 0; i < size; i++){
		auto item = treeWidget->topLevelItem(i);
		QVariantMap resData;
		if (item->data(0, Qt::UserRole).type() == QVariant::Bool){
			QVariantList listChild;
			QDir dir;
			QString outDir = outputDir + item->text(0) + "/";
			QString relaDir = relativeDir + item->text(0) + "/";
			dir.mkdir(outDir);
			copyResInItem(listChild, outputImage, outDir, relaDir, item, treeWidget);
			resData.insert("name", FontChina::G2U(item->text(0).toLocal8Bit()));
			resData.insert("res_List", listChild);
			resData.insert("isDir", true);
		}
		else{
			resData.insert("isDir", false);
			if (outputImage){
				QString path = ImgTransPath::getRealPathByIndex(item->data(0, Qt::UserRole).toInt());
				QString toFile = this->outputDir + treeWidget->getResItemRelaPath(item);
				resData.insert("res", FontChina::G2U(toFile.toLocal8Bit()));
				resData.insert("relative", FontChina::G2U(treeWidget->getResItemRelaPath(item).toLocal8Bit().data()));
				if (!QFile::exists(toFile)){
					QFile::copy(path, toFile);
				}
			}
			else{
				QString path = ImgTransPath::getRealPathByIndex(item->data(0, Qt::UserRole).toInt());
				resData.insert("res", FontChina::G2U(path.toLocal8Bit().data()));
			}
		}
		list.append(resData);
	}
}

void QTOutput::copyResInItem(QVariantList& list, bool outputImage, QString outputDir, QString relativeDir, QTreeWidgetItem* item, ImageTreeWidget* treeWidget)
{
	int size = item->childCount();
	for (int i = 0; i < size; i++){
		auto itemChild = item->child(i);
		QVariantMap resData;
		if (itemChild->data(0, Qt::UserRole).type() == QVariant::Bool){
			QVariantList listChild;
			QDir dir;
			QString outDir = outputDir + itemChild->text(0) + "/";
			QString relaDir = relativeDir + itemChild->text(0) + "/";
			dir.mkdir(outDir);
			copyResInItem(listChild, outputImage, outputDir, relativeDir, itemChild, treeWidget);
			resData.insert("name", FontChina::G2U(itemChild->text(0).toLocal8Bit()));
			resData.insert("res_List", listChild);
			resData.insert("isDir", true);
		}
		else{
			resData.insert("isDir", false);
			if (outputImage){
				QString path = ImgTransPath::getRealPathByIndex(itemChild->data(0, Qt::UserRole).toInt());
				QString toFile;
				toFile = this->outputDir + treeWidget->getResItemRelaPath(itemChild);
				resData.insert("res", FontChina::G2U(toFile.toLocal8Bit()));
				resData.insert("relative", FontChina::G2U(treeWidget->getResItemRelaPath(itemChild).toLocal8Bit().data()));
				if (!QFile::exists(toFile)){
					QFile::copy(path, toFile);
				}
			}
			else{
				QString path = ImgTransPath::getRealPathByIndex(itemChild->data(0, Qt::UserRole).toInt());
				resData.insert("res", FontChina::G2U(path.toLocal8Bit().data()));
			}
		}
		list.append(resData);
	}
}

void QTOutput::copyFrameTree(QVariantList& list, bool outputImage, QString outputDir, QString relativeDir, AnimationTreeWidget* treeWidget)
{
	int size = treeWidget->topLevelItemCount();
	for (int i = 0; i < size; i++){
		auto item = treeWidget->topLevelItem(i);
		QVariantMap resData;
		if (item->data(0, Qt::UserRole).type() == QVariant::Bool){
			QVariantList listChild;
			QDir dir;
			QString outDir = outputDir + treeWidget->getAnimationName(item) + "/";
			QString relaDir = relativeDir + treeWidget->getAnimationName(item) + "/";
			dir.mkdir(outDir);
			copyFrameInItem(listChild, outputImage, outDir, relaDir, item, treeWidget);
			resData.insert("animationName", FontChina::G2U(treeWidget->getAnimationName(item).toUtf8().data()));
			resData.insert("frame_List", listChild);
			resData.insert("isDir", true);
			list.append(resData);
		}	
	}
}

void QTOutput::copyFrameInItem(QVariantList& list, bool outputImage, QString outputDir, QString relativeDir, QTreeWidgetItem* item, AnimationTreeWidget* treeWidget)
{
	int size = item->childCount();
	for (int i = 0; i < size; i++){
		auto itemChild = item->child(i);
		QVariantMap resData;
		if (itemChild->data(0, Qt::UserRole).type() == QVariant::Bool){
			QVariantList listChild;
			QDir dir;
			QString outDir = outputDir + treeWidget->getAnimationName(itemChild) + "/";
			QString relaDir = relativeDir + treeWidget->getAnimationName(itemChild) + "/";
			dir.mkdir(outDir);
			copyFrameInItem(listChild, outputImage, outputDir, relativeDir, itemChild, treeWidget);
			resData.insert("animationName", FontChina::G2U(treeWidget->getAnimationName(itemChild).toUtf8().data()));
			resData.insert("frame_List", listChild);
			resData.insert("isDir", true);
		}
		else{
			resData.insert("isDir", false);
			if (outputImage){
				QString path = ImgTransPath::getRealPathByIndex(itemChild->data(0, Qt::UserRole).toInt());
				QString toFile;
				toFile = this->outputDir + treeWidget->getResItemRelaPath(itemChild);
				resData.insert("res", FontChina::G2U(toFile.toLocal8Bit()));
				resData.insert("relative", FontChina::G2U(treeWidget->getResItemRelaPath(itemChild).toLocal8Bit().data()));
				if (!QFile::exists(toFile)){
					QFile::copy(path, toFile);
				}
			}
			else{
				QString path = ImgTransPath::getRealPathByIndex(itemChild->data(0, Qt::UserRole).toInt());
				resData.insert("res", FontChina::G2U(path.toLocal8Bit().data()));
			}
		}
		list.append(resData);
	}
}

void QTOutput::writeResourceData(QVariantMap& document, bool outputimage)
{
	{
		//TemplateList
		QVariantList templateList;
		TemplateView* view = dynamic_cast<TemplateView*>(g_TemplateView);
		QStringList list = view->getAllTemplateFile();
		if (outputimage){
			auto templatePath = outputDir;
			templatePath += "template";
			QDir* dir = new QDir();
			if (!dir->exists(templatePath)){
				dir->mkdir(templatePath);
			}
			delete dir;
		}
		for (int i = 0; i < list.size(); i++){
			//模板的导出有问题
			QVariantMap templateData;
			if (outputimage){
				QString templateFile = outputDir + "template/" + list.at(i).section("/", -1);
				resetTemplate(list.at(i), templateFile, outputRootDir, true);
				templateData.insert("template", FontChina::G2U(templateFile.toLocal8Bit().data()));
				templateData.insert("relative", FontChina::G2U((g_c_CopyDir + "Template/" + list.at(i).section("/", -1)).toLocal8Bit().data()));
			}
			else{
				QString templateFile = g_templateDir.c_str();
				templateFile += "/";
				templateFile += list.at(i).section("/", -1);
				templateData.insert("template", FontChina::G2U(templateFile.toLocal8Bit().data()));
				templateData.insert("relative", FontChina::G2U(("Template/" + list.at(i).section("/", -1)).toLocal8Bit().data()));
			}
			templateData.insert("md5", view->getItem(i)->getMD5());
			templateList.append(templateData);
		}
		document.insert("template_list", templateList);
	}
	{
		if (outputimage){
			resourcePath = outputRootDir;
			resourcePath += g_c_relaResourceDir;
		}
		else{
			resourcePath = searthPath;
			resourcePath += g_c_relaResourceDir;
		}
		QDir dir;
		if (!dir.exists(resourcePath)){
			dir.mkdir(resourcePath);
		}
		//ResourceView List
		QVariantList resourceList;
		auto resourceView = static_cast<ResourceView*>(g_resourceView);
		ImageTreeWidget* resTreeWidget = dynamic_cast<ImageTreeWidget*>(resourceView->getTreeWidget());
		copyResTree(resourceList, outputimage, resourcePath, g_c_relaResourceDir, resTreeWidget);
		document.insert("res_tree", resourceList);
	}

	{
		if (outputimage){
			exteralPath = outputRootDir;
			exteralPath += g_c_relaExteralDir;
		}
		else{
			exteralPath = searthPath;
			exteralPath += g_c_relaExteralDir;
		}
		QDir dir;
		if (!dir.exists(exteralPath)){
			dir.mkdir(exteralPath);
		}
		QVariantList extralList;
		auto resourceView = static_cast<ExteralView*>(g_exteralView);
		ExteralTreeWidget* resTreeWidget = dynamic_cast<ExteralTreeWidget*>(resourceView->getTreeWidget());
		copyResTree(extralList, outputimage, exteralPath, g_c_relaExteralDir, resTreeWidget);
		document.insert("exteral_tree", extralList);
	}

	{
		if (outputimage){
			framePath = outputRootDir;
			framePath += g_c_relaFrameDir;
		}
		else{
			framePath = searthPath;
			framePath += g_c_relaFrameDir;
		}
		QDir dir;
		if (!dir.exists(framePath)){
			dir.mkdir(framePath);
		}
		QVariantList frameList;
		auto resourceView = static_cast<AnimationView*>(g_AnimationView);
		AnimationTreeWidget* resTreeWidget = dynamic_cast<AnimationTreeWidget*>(resourceView->getTreeWidget());
		copyFrameTree(frameList, outputimage, framePath, g_c_relaFrameDir, resTreeWidget);
		document.insert("frame_tree", frameList);
	}
}

void QTOutput::writeTilesData(QVariantMap& document, bool outputimage)
{
	MyLogger::getInstance()->addInfo("the QTOutput::OutputJsonConf be called, start output grid");
	HelloWorld* scene = (HelloWorld*)g_scene;
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	QVariantList tiles;
	//当前已经计算过的格子的数量
	for (int i = 0; i < scene->row; i++){
		for (int j = 0; j < scene->col; j++){
			if (scene->gridManagerLayer->getBBvec() && scene->gridManagerLayer->getBlock(i, j)){
				QVariantMap attribute;
				if (scene->gridManagerLayer->getBlock(i, j) && scene->gridManagerLayer->getBlock(i, j)->getType() == Type_Box){
					auto block = dynamic_cast<BoxBody*>(scene->gridManagerLayer->getBlock(i, j));
					attribute.insert("type", 0);
					attribute.insert("way", block->getWay());
				}
				else if (scene->gridManagerLayer->getBlock(i, j) && scene->gridManagerLayer->getBlock(i, j)->getType() == Type_Italic){
					attribute.insert("type", 1);
				}
				else if (scene->gridManagerLayer->getBlock(i, j) && scene->gridManagerLayer->getBlock(i, j)->getType() == Type_Fluid){
					attribute.insert("type", 2);
				}
				if (scene->gridManagerLayer->getBlock(i, j)->getType() == Type_Italic){
					auto block = dynamic_cast<ItalicBody*>(scene->gridManagerLayer->getBlock(i, j));
					attribute.insert("direction", block->getDrawType());
					attribute.insert("pos1_x", block->getPos1().x);
					attribute.insert("pos1_y", block->getPos1().y);
					attribute.insert("pos2_x", block->getPos2().x);
					attribute.insert("pos2_y", block->getPos2().y);
					attribute.insert("start_x", block->getStartPos().x);
					attribute.insert("start_y", block->getStartPos().y);
					attribute.insert("end_x", block->getEndPos().x);
					attribute.insert("end_y", block->getEndPos().y);
				}
				QVariantMap pos;
				pos.insert("x", (int)scene->gridManagerLayer->getBlock(i, j)->getPositionX() / scene->gridWidth);
				if (g_CocosWindowInitSize.height() > 640){
					auto size = getNewWindowSize();
					if (size.height() > 640){
						pos.insert("y", (int)(glview->getFrameSize().height - scene->gridManagerLayer->getBlock(i, j)->getPositionY()) / scene->gridHeight);
					}
					else{
						pos.insert("y", (int)(g_CocosWindowInitSize.height() - scene->gridManagerLayer->getBlock(i, j)->getPositionY()) / scene->gridHeight);
					}
				}
				else{
					auto size = getNewWindowSize();
					if (size.height() > 640){
						pos.insert("y", (int)(g_CocosWindowInitSize.height() - scene->gridManagerLayer->getBlock(i, j)->getPositionY()) / scene->gridHeight);
					}
					else{
						pos.insert("y", (int)(glview->getFrameSize().height - scene->gridManagerLayer->getBlock(i, j)->getPositionY()) / scene->gridHeight);
					}
				}
				attribute.insert("pos", pos);
				tiles.append(attribute);
			}
		}
	}
	document.insert("tiles", tiles);
}

void QTOutput::writeAllLayerData(QVariantMap& document, bool outputimage)
{
	HelloWorld* scene = (HelloWorld*)g_scene;
	std::vector<ImageSpriteLayer*> layers = scene->getAllImageLayer();
	QVariantList alllayer;
	for (int i = 0; i < layers.size(); i++){
		QVariantMap layerdata;
		auto layer = dynamic_cast<ImageSpriteLayer*>(layers.at(i));
		if (!layer){
			MyLogger::getInstance()->addError("Error, layer is null, layer index is " + QString::number(i));
		}
		writeSimpleLayerData(layer, layerdata, outputimage, i);
		alllayer.append(layerdata);
	}
	MyLogger::getInstance()->addInfo("all layer data insert finished");
	document.insert("alllayer", alllayer);
}

void QTOutput::writeAllLayerFilesData(QVariantMap& document, bool outputimage)
{
	HelloWorld* scene = (HelloWorld*)g_scene;
	std::vector<ImageSpriteLayer*> layers = scene->getAllImageLayer();
	QVariantList allLayerFile;
	QDir dir;
	if (!dir.exists(outputRootDir + "layers")){
		dir.mkdir(outputRootDir + "layers");
	}
	for (int i = 0; i < layers.size(); i++){
		QVariantMap layernamemap;
		auto layer = dynamic_cast<ImageSpriteLayer*>(layers.at(i));
		if (!layer){
			MyLogger::getInstance()->addError("Error, layer is null, layer index is " + QString::number(i));
		}
		QString tagname = layer->getTagName().c_str();
		QString filename = "layers/" + tagname + ".lyr";
		layernamemap.insert("layerfile", filename);
		allLayerFile.append(layernamemap);
		QVariantMap layerMap;
		writeSimpleLayerData(layer, layerMap, outputimage, i);
		auto convert = new convertMapToJsonFile();
		convert->setDocument(&layerMap);
		filename = outputRootDir + filename;
		convert->setFile(filename);
		convert->startConvert();
		convert->deleteData();
		delete convert;
		convertMapToJsonFile::changeFileToUtf8(filename);
	}
	MyLogger::getInstance()->addInfo("all layer data insert finished");
	document.insert("layerFileList", allLayerFile);
}

void QTOutput::writeSimpleLayerData(ImageSpriteLayer* layer, QVariantMap& layerdata, bool outputimage, int i)
{
	MyLogger::getInstance()->addInfo("the QTOutput::OutputJsonConf be called, output layer name is " + QString(layer->getTagName().c_str()));
	QVariantList images;
	layerdata.insert("index", i);
	layerdata.insert("tagname", FontChina::G2U(layer->getTagName().c_str()));
	layerdata.insert("z_order", layer->getZOrder());
	layerdata.insert("x", layer->getPositionX());
	layerdata.insert("y", layer->getPositionY());
	layerdata.insert("visible", layer->isVisible());
	layerdata.insert("state", layer->isSportState());
	layerdata.insert("isLock", layer->isLock());
	layerdata.insert("moveScaleX", layer->getMoveScaleX());
	layerdata.insert("moveScaleY", layer->getMoveScaleY());
	layerdata.insert("sceneScaleFluence", layer->getSceneScaleFluence());
	layerdata.insert("filterType", layer->getFilterType());
	layerdata.insert("dontOutputState", layer->isDontOutput());
	auto scene = (HelloWorld*)(g_scene);
	auto drawData = scene->getForeManager()->getLayerMoveRangeManage()->getLayerData(layer->getTagName());
	layerdata.insert("drawVisible", drawData->drawnode->isVisible());
	layerdata.insert("col1", drawData->col1);
	layerdata.insert("row1", drawData->row1);
	layerdata.insert("col2", drawData->col2);
	layerdata.insert("row2", drawData->row2);
	Vector<Node*> sprites = layer->getAllSpriteNode();
	MyLogger::getInstance()->addInfo("the QTOutput::OutputJsonConf be called, start output sprite, sprite count is " + QString::number(sprites.size()));
	for (int j = 0; j < sprites.size(); j++){
		auto sprite = dynamic_cast<ImageSprite*>(sprites.at(j));
		QVariantMap image;
		QString relat = ImgTransPath::getRelativePathByIndex(sprite->getImageIndex());
		image.insert("relative", relat);
		image.insert("pos_x", sprite->getPositionX());
		image.insert("pos_y", sprite->getPositionY());
		image.insert("z_order", sprite->getZOrder());
		image.insert("tagname", FontChina::G2U(sprite->getTagName().c_str()));
		image.insert("visible", sprite->isVisible());
		image.insert("copyCount", sprite->getCopyCount());
		image.insert("id", sprite->getId());
		if (sprite->isHasAnimation()){
			image.insert("animationName", sprite->getAnimationName().c_str());
		}
		if (outputimage){
			setPropertiesForSprite(image, sprite, true, outputDir, g_c_CopyDir);
		}
		else{
			setPropertiesForSprite(image, sprite);
		}
		images.append(image);
	}
	layerdata.insert("images", images);
}
