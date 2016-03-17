#pragma once
#include "PackerCommandThread.h"
#include "ImageSpriteLayer.h"

class OutputPackerStaticThread : public PackerCommandThread
{
public:
	OutputPackerStaticThread();
	~OutputPackerStaticThread();

	virtual void run() override;

	virtual int generateCommand(QStringList fileList, QString name, QString dir, QString plat, QString form, int _type);
	virtual int getCommandGenerateCount();
	virtual int getFileCount(QString name, QString outdir);
	virtual bool recursionVec(std::vector<ImageSpriteLayer*>& vecLayer, int i, int& index);
	virtual bool reCursionVecRoot(std::vector<ImageSpriteLayer*>& vecLayer, int i);
	virtual int writeAStaticLayer(std::vector<ImageSpriteLayer*>& vecLayer, QVector<ImageSpriteLayer*>* qvec, int bigIndex, int smallIndex, int& static_index);
	virtual void addToVec(std::vector<std::string>& sourceVec, std::vector<std::string>& dstVec);
	virtual std::vector<std::string> subToVec(std::vector<ImageSpriteLayer*>& vecLayer, int startIndex, int endedIndex);
	unsigned int getImagesSize(std::vector<std::string>& vec);
	QStringList convertToStringList(std::vector<std::string>& vec);
	QStringList packerConvertStringList(QStringList list, QString outdir, QString prefix);

	int getNewStaticTexturePackerIndex();
private:
	int _type;
	int index;
};

