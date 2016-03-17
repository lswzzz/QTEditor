#pragma once
#include "BaseDockWidget.h"
#include "QPushButton"
#include "QComboBox"
#include "QLineEdit"
#include "QCheckBox"

class ConfigView : public BaseDockWidget
{
	Q_OBJECT
public:
	ConfigView(char* title, QSize size);
	~ConfigView();
	void setScaleTextValue(float scale);
	QLineEdit* getSceneScale(){ return sceneScale; };
	bool getLayerStructureState();
	void setLayerStructureState(bool state);
	void setGridColorR(int r);
	void setGridColorG(int g);
	void setGridColorB(int b);
	void setGridColorA(int a);
	void setColorR(int r);
	void setColorG(int g);
	void setColorB(int b);
	
private slots:
	void scaleTextRedo();
	void minScaleRedo();
	void maxScaleRedo();
	void on_setOptionButton_clicked();
	void showWidthRedo();
	void showHeightRedo();
	void resetScenePos_clicked();
	void setColorButton_clicked();
	void setGridColorButton_clicked();
protected:
	void __connect();
private:
	QLineEdit* sceneScale;
	QLineEdit* minScale;
	QLineEdit* maxScale;
	QLineEdit* showWidth;
	QLineEdit* showHeight;
	QLineEdit* rColor;
	QLineEdit* gColor;
	QLineEdit* bColor;
	QPushButton* setColorButton;
	QLineEdit* gridRColor;
	QLineEdit* gridGColor;
	QLineEdit* gridBColor;
	QLineEdit* gridAColor;
	QPushButton* setGridColorButton;
	QComboBox* optionBox;
	QPushButton* setOption;
	QPushButton* resetScenePosButton;
	QCheckBox* isLayerStructure;
};

