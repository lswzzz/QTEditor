#pragma once
#include "cocos2d.h"

USING_NS_CC;

class ImageTextureManager
{
public:
	ImageTextureManager();
	virtual ~ImageTextureManager();

	void clearAllMap();
	void setImageIndexMap(std::map<std::string, int> map_);
	Texture2D* getTexture(std::string image);
	cocos2d::Rect getRect(std::string image);
	bool imageIsRotate(std::string image);

	int getImageIndex(std::string image);
	std::string getPng(std::string image);
	void addTextureToMap(std::string image);
	void setPackerImage(std::string filename, int count);

	void addToRotatedMap(std::string image, std::string rotated);
	void addToTextureMap(std::string image, Texture2D* texture);
	void addToRectMap(std::string image, cocos2d::Rect rect);

	std::map<std::string, Texture2D*> textureMap;
    std::map<std::string, cocos2d::Rect> rectMap;
	std::map<int, std::string> plistMap;
	std::map<int, std::string> pngMap;
	std::map<std::string, int> imageIndexMap;
	std::map<int, CCDictionary*> dictionarysMap;
	std::map<int, CCDictionary*> contentsMap;
	std::map<std::string, bool> retatedMap;
	std::string layername;
};

