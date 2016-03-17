#include "ImageTextureManager.h"
#include <QString>
#include "MyLogger.h"

ImageTextureManager::ImageTextureManager()
{
}

ImageTextureManager::~ImageTextureManager()
{
	clearAllMap();
}

void ImageTextureManager::setImageIndexMap(std::map<std::string, int> map_)
{
	if (map_.size() == 0){
		MyLogger::getInstance()->addError("Error ImageTextureManager::setImageIndexMap, map size == 0, layername is " + layername);
	}
	imageIndexMap = map_;
}

int ImageTextureManager::getImageIndex(std::string image)
{
	auto iter = imageIndexMap.find(image);
	if (iter != imageIndexMap.end()){
		return iter->second;
	}
	return -1;
}

std::string ImageTextureManager::getPng(std::string image)
{
	auto iter = pngMap.find(getImageIndex(image));
	if (iter != pngMap.end()){
		return iter->second;
	}
	return "";
}

Texture2D* ImageTextureManager::getTexture(std::string image)
{
	auto iter = textureMap.find(image);
	if (iter != textureMap.end()){
		return iter->second;
	}
	return nullptr;
}

Rect ImageTextureManager::getRect(std::string image)
{
	auto iter = rectMap.find(image);
	if (iter != rectMap.end()){
		return iter->second;
	}
	return Rect();
}

void ImageTextureManager::addTextureToMap(std::string image)
{
	int index = getImageIndex(image);
	auto content = contentsMap[index];
	CCDictionary* contentImage = (CCDictionary*)content->objectForKey(image);
	CCString* object = (CCString*)contentImage->objectForKey("frame");
	CCString* rotated = (CCString*)contentImage->objectForKey("rotated");
	QString str = object->getCString();
	str.replace("}", "{");
	str.replace(",", "{");
	int x = str.section("{", 2, 2).toInt();
	int y = str.section("{", 3, 3).toInt();
	int width = str.section("{", 6, 6).toInt();
	int height = str.section("{", 7, 7).toInt();
	auto rect = Rect(x, y, width, height);
	auto texture = TextureCache::getInstance()->getTextureForKey(pngMap[index]);
	addToRotatedMap(image, rotated->getCString());
	addToTextureMap(image, texture);
	addToRectMap(image, rect);
}

void ImageTextureManager::setPackerImage(std::string filename, int count)
{
	if (filename == "" || count == 0){
		MyLogger::getInstance()->addError("Error texture count is 0 or filename is null, layername is " + layername);
	}
	for (int i = 0; i < count; i++){
		std::string plist = filename + QString::number(i).toStdString() + ".plist";
		std::string png = filename + QString::number(i).toStdString() + ".png";
		pngMap.insert(std::pair<int, std::string>(i, png));
		plistMap.insert(std::pair<int, std::string>(i, plist));
		auto dictionary = (CCDictionary*)CCDictionary::createWithContentsOfFile(plist.c_str());
		auto content = (CCDictionary*)dictionary->objectForKey("frames");
		dictionarysMap.insert(std::pair<int, CCDictionary*>(i, dictionary));
		contentsMap.insert(std::pair<int, CCDictionary*>(i, content));
	}
}

void ImageTextureManager::addToTextureMap(std::string image, Texture2D* texture)
{
	textureMap.insert(std::pair<std::string, Texture2D*>(image, texture));
}

void ImageTextureManager::addToRectMap(std::string image, Rect rect)
{
	rectMap.insert(std::pair<std::string, Rect>(image, rect));
}

bool ImageTextureManager::imageIsRotate(std::string image)
{
	auto iter = retatedMap.find(image);
	if (iter != retatedMap.end()){
		return iter->second;
	}
	return false;
}

void ImageTextureManager::addToRotatedMap(std::string image, std::string rotated)
{
	if (rotated == "false"){
		retatedMap.insert(std::pair<std::string, bool>(image, false));
	}
	else{
		retatedMap.insert(std::pair<std::string, bool>(image, true));
	}
}

void ImageTextureManager::clearAllMap()
{
	textureMap.clear();
	rectMap.clear();
	plistMap.clear();
	pngMap.clear();
	imageIndexMap.clear();
	dictionarysMap.clear();
	contentsMap.clear();
	retatedMap.clear();
	layername = "";
}