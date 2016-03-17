#include "ImgTransPath.h"
#include "cocos2d.h"
#include "GlobalConstant.h"
#include "ResMap.h"

using namespace cocos2d;

QString ImgTransPath::resListPath="";
QString ImgTransPath::resImageRootPath="";
QString ImgTransPath::resExteralRootPath="";

ImgTransPath::ImgTransPath()
{
}


ImgTransPath::~ImgTransPath()
{
}

QString ImgTransPath::getRealPathByIndex(int index)
{
	return ResMap::getImageRealPath(index);
}

QString ImgTransPath::getRelativePathByIndex(int index)
{
	return ResMap::getImageRelativePath(index);
}

void ImgTransPath::updatePath()
{
	QString searthPath = FileUtils::getInstance()->getSearchPaths().at(0).c_str();
	resListPath = searthPath + g_c_rela_ResourceListDir;
	resImageRootPath = searthPath + g_c_relaResourceDir;
	resExteralRootPath = searthPath + g_c_relaExteralDir;
}

QString ImgTransPath::getListImagePathByIndex(int index)
{
	QString imageType = ResMap::getImageType(index);
	if (imageType != ""){
		QString path = resListPath + QString::number(index) + "." + imageType;
		return path;
	}
	return "";
}

int ImgTransPath::getListImageIndex(QString realImg)
{
	return ResMap::getImageIndexByRelaPath(realImg);
}

QString ImgTransPath::getListPngName(int index)
{
	return  QString::number(index) + "." + ResMap::getImageType(index);
}

QString ImgTransPath::getListPngName(ImageSprite* sprite)
{
	int index = sprite->getImageIndex();
	return getListPngName(index);
}

QString ImgTransPath::getListImagePath(ImageSprite* sprite)
{
	int index = sprite->getImageIndex();
	return getListImagePathByIndex(index);
}

QString ImgTransPath::getRealImageName(int index)
{
	return getRealPathByIndex(index).section("/", -1);
}

QString ImgTransPath::getRealImageName(ImageSprite* sprite)
{
	int index = sprite->getImageIndex();
	return getRealImageName(index);
}

QString ImgTransPath::getRealImagePath(ImageSprite* sprite)
{
	int index = sprite->getImageIndex();
	return getRealPathByIndex(index);
}

int ImgTransPath::getIndexByListImage(QString listImg)
{
	QString str = listImg.section("/", -1);
	return str.section(".", 0, -2).toInt();
}

QString ImgTransPath::getListImagePathByRelativeImagePath(QString relaPath)
{
	int index = ResMap::relaImagePathIndexMap.value(relaPath);
	return getListImagePathByIndex(index);
}

QString ImgTransPath::getNewRelaPath(QString img)
{
	return ResMap::oldPathMap.value(img);
}

QString ImgTransPath::getOutputListPngName(ImageSprite* sprite, QString prefix)
{
	auto str = getListPngName(sprite);
	return prefix + str;
}