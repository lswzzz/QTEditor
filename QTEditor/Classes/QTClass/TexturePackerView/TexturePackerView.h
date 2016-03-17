#pragma once
#include <BaseDockWidget.h>
#include <qlistwidget.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qtextedit.h>
#include <qplaintextedit.h>
#include "cocos2d.h"
#include "OutputInfo.h"

class ImageSpriteLayer;
using namespace cocos2d;
//http://www.cnblogs.com/flyFreeZn/p/3587770.html
class TexturePackerView : public BaseDockWidget
{
	Q_OBJECT
public:
	TexturePackerView(char* title, QSize size);
	~TexturePackerView();

	QStringList convertToStringList(std::vector<std::string>& vec);
	QStringList getUsingImage();
	std::vector<QStringList> getStaticLayerImage();
	int checkFile(QString outdir, QString outName);
	void PackerAsyn(QStringList fileList, QString outputName, QString outputDir, Node* layer, bool needShowLabel = true, QString platform = "win32", QString format = "png");
	bool prePacker(QStringList fileList, QString name = "pre_Packer_Resource", QString dir = "../Resources", QString plat = "win32", QString form = "png");
	bool PackerFrame(QString dir);
	void PreViewSave(char* path, char* name);
	QVector<QVector<ImageSpriteLayer*>*> getVecLayer(){ return m_vecLayer; };
	void deleteVecLayer();
	int getPngCount(QString outdir, QString outName);
private:
	bool outputPacker(QString resDir, QString name, QString dir, QString plat, QString form, int _type);
	int outputTest(QString resDir, QString name, QString dir, QString plat, QString form, int _type);
	int outputTestCommnad(QString command, QString name, QString dir);
	//导出静态层的时候进行测试是否超出了数量
	int getOutputTestPngCount(QString name, QString dir);
	void outputStaticLayer();
	bool recursionVec(std::vector<ImageSpriteLayer*>& vecLayer, int i, int& index);
	void addToVec(std::vector<std::string>& sourceVec, std::vector<std::string>& dstVec);
	std::vector<std::string> subToVec(std::vector<ImageSpriteLayer*>& vecLayer, int startIndex, int endedIndex);
	int writeAStaticLayer(std::vector<ImageSpriteLayer*>& vecLayer, QVector<ImageSpriteLayer*>* qvec, int bigIndex, int smallIndex, int& static_index);
	void checkOutputCount();
	void runPacker(QString command, int type_);
	bool ImageOptimize(QString file, QString outputName, QString outputDir);
	void addFrameConfigFile(QString dir, QString parentplist, QString parentpng, QString filename);
	
	void ThreadUnPacker();
	unsigned int getImagesSize(std::vector<std::string>& vec);

	QString copyFileAndReturnDir(QStringList list, QString outdir, QString prefix);
	void createDirAndCopyFile(QString dir, QStringList fileList);
	QStringList getDirRootFile(QString dir);
	void writeALayerVec(QVector<ImageSpriteLayer*>* vec);

private slots:
	void on_dirButton_clicked();
	void on_sureButton_clicked();
	void prePackerThread();
	
	void on_chooseFile_clicked();
	void on_chooseDir_clicked();
	void on_unPackerButton_clicked();
signals:

private:
	QComboBox* format;
	QComboBox* platform;
	QTextEdit* outputName;
	QPlainTextEdit* outputDir;
	QPushButton* sureButton;
	QPushButton* dirButton;


	//unpacker
	QPushButton* chooseFile;
	QLabel* unpackerFile;
	QPushButton* chooseDir;
	QLabel* unpackerDir;
	QPushButton* unPackerButton;
	std::mutex g_mutex;
	std::mutex writeToVecMutex;
	QVector<QVector<ImageSpriteLayer*>*> m_vecLayer;

	friend class OutputPackerThread;
	friend class OutputPackerStaticThread;
};

