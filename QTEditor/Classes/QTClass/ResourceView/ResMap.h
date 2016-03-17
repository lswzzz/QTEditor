#pragma once
#include "QHash"
#include "QString"
#include "cocos2d.h"

using namespace cocos2d;

class ResMap
{
public:

	static QString getImageRealPath(int key);
	static QString getImageRelativePath(int key);
	static QString getImageType(int key);
	static int getImageIndexByRelaPath(QString key);
	static Texture2D* getSinglePngTexture(int index);

	static void addRelaImagePathIndexInfo(QString key, int value);
	static void addImageTypeInfo(int key, QString value);
	static void addRealImagePathInfo(int key, QString value);
	static void addRelaImagePathInfo(int key, QString value);
	static void addSinglePngTextureInfo(int key, Texture2D* value);

	static void removeRelaImagePathIndexInfo(QString key);
	static void removeImageTypeInfo(int key);
	static void removeRealImagePathInfo(int key);
	static void removeRelaImagePathInfo(int key);
	static void removeSinglePngTextureInfo(int key);

	static void resClear();
	
	static int getImageIndex(){	return imageIndex;};
	static void addImageIndex(){ ++imageIndex; };

	static QHash<QString, QString> oldPathMap;
private:

	//��ʵ�ļ����Ŀ¼��Ӧ���±�
	static QHash<QString, int> relaImagePathIndexMap;
	//�����±��ӦͼƬ�ĺ�׺
	static QHash<int, QString> imageTypeMap;
	//���±�ӳ�䵽��ʵ���ļ�Ŀ¼
	static QHash<int, QString> realImagePathMap;
	//���±�ӳ�䵽��Ե��ļ�Ŀ¼
	static QHash<int, QString> relaPathMap;

	static QHash<int, Texture2D*> textureMap;
	static int imageIndex;
	friend class ImgTransPath;
};

