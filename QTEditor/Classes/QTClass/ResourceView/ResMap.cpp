#include "ResMap.h"

QHash<QString, int> ResMap::relaImagePathIndexMap;

QHash<int, QString> ResMap::imageTypeMap;

QHash<int, QString> ResMap::realImagePathMap;

QHash<int, QString> ResMap::relaPathMap;

QHash<QString, QString> ResMap::oldPathMap;

QHash<int, Texture2D*> ResMap::textureMap;

int ResMap::imageIndex;

void ResMap::resClear()
{
	imageIndex = 0;
	relaImagePathIndexMap.clear();
	imageTypeMap.clear();
	realImagePathMap.clear();
	relaPathMap.clear();
}

QString ResMap::getImageRealPath(int key)
{
	return realImagePathMap.value(key);
}

QString ResMap::getImageRelativePath(int key)
{
	return relaPathMap.value(key);
}

QString ResMap::getImageType(int key)
{
	return imageTypeMap.value(key);
}

int ResMap::getImageIndexByRelaPath(QString key)
{
	return relaImagePathIndexMap.value(key);
}

Texture2D* ResMap::getSinglePngTexture(int index)
{
	return textureMap.value(index);
}

void ResMap::addRelaImagePathIndexInfo(QString key, int value)
{
	relaImagePathIndexMap[key] = value;
}

void ResMap::addImageTypeInfo(int key, QString value)
{
	imageTypeMap[key] = value;
}

void ResMap::addRealImagePathInfo(int key, QString value)
{
	realImagePathMap[key] = value;
}

void ResMap::addRelaImagePathInfo(int key, QString value)
{
	relaPathMap[key] = value;
}

void ResMap::addSinglePngTextureInfo(int key, Texture2D* value)
{
	textureMap[key] = value;
}

void ResMap::removeRelaImagePathIndexInfo(QString key)
{
	relaImagePathIndexMap.remove(key);
}

void ResMap::removeImageTypeInfo(int key)
{
	imageTypeMap.remove(key);
}

void ResMap::removeRealImagePathInfo(int key)
{
	realImagePathMap.remove(key);
}

void ResMap::removeRelaImagePathInfo(int key)
{
	relaPathMap.remove(key);
}

void ResMap::removeSinglePngTextureInfo(int key)
{
	textureMap.remove(key);
}