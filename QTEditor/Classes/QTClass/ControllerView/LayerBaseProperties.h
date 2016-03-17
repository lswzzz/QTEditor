#pragma once
#include "BaseDockWidget.h"
#include "ImageSpriteLayer.h"
#include <QLineEdit>
#include <QComboBox>
#include "QLayout"

class LayerBaseProperties : public BaseDockWidget
{
	Q_OBJECT
public:
	LayerBaseProperties(char* title, QSize size);
	~LayerBaseProperties();
	void setTargetLayer(ImageSpriteLayer* layer);
	
	void setWidgetValue();
private slots:
	void moveScaleXEditFinished();
	void moveScaleYEditFinished();
	void sceneScalefluenceEditFinished();
	void EditFinishedDrawLayerRange();
	void filterTypeChange(int type_);
	void showTypeChange(int type_);
protected:
	void _connect();
	
protected:
	ImageSpriteLayer* targetLayer;

	bool InternalOperation;
	QLineEdit* moveScaleX;
	QLineEdit* moveScaleY;
	QLineEdit* sceneScale;
	QLineEdit* startPosX;
	QLineEdit* startPosY;
	QLineEdit* endedPosX;
	QLineEdit* endedPosY;
	QComboBox* showTypeComboBox;
	QComboBox* filterType;
};

