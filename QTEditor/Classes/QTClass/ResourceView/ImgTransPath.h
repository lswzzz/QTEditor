#pragma once
#include "QString"
#include "ImageSprite.h"

class ImgTransPath
{
public:
	ImgTransPath();
	~ImgTransPath();

	static QString	getRealPathByIndex(int index);
	static QString	getRelativePathByIndex(int index);
	static QString	getListImagePathByIndex(int index);
	static int		getListImageIndex(QString realImg);
	static QString	getListPngName(int index);
	static QString	getListPngName(ImageSprite* sprite);
	static QString	getListImagePath(ImageSprite* sprite);
	static QString	getRealImageName(int index);
	static QString	getRealImageName(ImageSprite* sprite);
	static QString	getRealImagePath(ImageSprite* sprite);
	static int		getIndexByListImage(QString listImg);
	static QString	getListImagePathByRelativeImagePath(QString relaPath);
	static QString	getOutputListPngName(ImageSprite* sprite, QString prefix);
	static QString	resListPath;
	static QString	resImageRootPath;
	static QString	resExteralRootPath;
	static void		updatePath();

	static QString	getNewRelaPath(QString img);
};

