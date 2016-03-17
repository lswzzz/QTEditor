#include "LayerBaseProperties.h"
#include "FontChina.h"
#include <QLabel>
#include "MyLogger.h"
#include "HelloWorldScene.h"

LayerBaseProperties::LayerBaseProperties(char* title, QSize size)
:BaseDockWidget(title, size, true)
{
	targetLayer = NULL;
	InternalOperation = false;
	QVBoxLayout* vlayout = new QVBoxLayout();
	QGridLayout* gridlayout = new QGridLayout();
	QLabel* label = new QLabel(FontChina::G2U("移动缩放X"));
	QLabel* label2 = new QLabel(FontChina::G2U("移动缩放Y"));
	QLabel* label3 = new QLabel(FontChina::G2U("场景缩放系数"));
	QLabel* label4 = new QLabel(FontChina::G2U("滤镜类型"));
	QLabel* label5 = new QLabel(FontChina::G2U("col1"));
	QLabel* label6 = new QLabel(FontChina::G2U("row1"));
	QLabel* label7 = new QLabel(FontChina::G2U("col2"));
	QLabel* label8 = new QLabel(FontChina::G2U("row2"));
	moveScaleX = new QLineEdit();
	moveScaleY = new QLineEdit();
	sceneScale = new QLineEdit();
	startPosX = new QLineEdit();
	startPosY = new QLineEdit();
	endedPosX = new QLineEdit();
	endedPosY = new QLineEdit();
	filterType = new QComboBox();
	showTypeComboBox = new QComboBox();
	filterType->addItem(FontChina::G2U("无"));
	filterType->addItem(FontChina::G2U("叠加"));
	filterType->addItem(FontChina::G2U("颜色减淡"));
	filterType->addItem(FontChina::G2U("覆盖"));
	showTypeComboBox->addItem(FontChina::G2U("隐藏"));
	showTypeComboBox->addItem(FontChina::G2U("显示"));
	gridlayout->addWidget(label, 0, 0);
	gridlayout->addWidget(moveScaleX, 0, 1);
	gridlayout->addWidget(label2, 0, 2);
	gridlayout->addWidget(moveScaleY, 0, 3);
	
	QGridLayout* glayout2 = new QGridLayout;
	glayout2->addWidget(label5, 1, 0);
	glayout2->addWidget(startPosX, 1, 1);
	glayout2->addWidget(label6, 1, 2);
	glayout2->addWidget(startPosY, 1, 3);
	glayout2->addWidget(label7, 2, 0);
	glayout2->addWidget(endedPosX, 2, 1);
	glayout2->addWidget(label8, 2, 2);
	glayout2->addWidget(endedPosY, 2, 3);
	glayout2->addWidget(showTypeComboBox, 2, 4);

	gridlayout->addLayout(glayout2, 1, 0, 2, 4);
	gridlayout->addWidget(label3, 3, 0);
	gridlayout->addWidget(sceneScale, 3, 1);
	gridlayout->addWidget(label4, 3, 2);
	gridlayout->addWidget(filterType, 3, 3);
	vlayout->addLayout(gridlayout);
	widget->setLayout(vlayout);
	_connect();
}

LayerBaseProperties::~LayerBaseProperties()
{
}

void LayerBaseProperties::_connect()
{
	connect(moveScaleX, SIGNAL(editingFinished()), this, SLOT(moveScaleXEditFinished()));
	connect(moveScaleY, SIGNAL(editingFinished()), this, SLOT(moveScaleYEditFinished()));
	connect(sceneScale, SIGNAL(editingFinished()), this, SLOT(sceneScalefluenceEditFinished()));
	connect(startPosX, SIGNAL(editingFinished()), this, SLOT(EditFinishedDrawLayerRange()));
	connect(startPosY, SIGNAL(editingFinished()), this, SLOT(EditFinishedDrawLayerRange()));
	connect(endedPosX, SIGNAL(editingFinished()), this, SLOT(EditFinishedDrawLayerRange()));
	connect(endedPosY, SIGNAL(editingFinished()), this, SLOT(EditFinishedDrawLayerRange()));
	connect(filterType, SIGNAL(currentIndexChanged(int)), this, SLOT(filterTypeChange(int)));
	connect(showTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showTypeChange(int)));
}

void LayerBaseProperties::moveScaleXEditFinished()
{
	if (!InternalOperation && targetLayer){
		targetLayer->setMoveScaleX(moveScaleX->text().toDouble());
	}
}

void LayerBaseProperties::moveScaleYEditFinished()
{
	if (!InternalOperation && targetLayer){
		targetLayer->setMoveScaleY(moveScaleY->text().toDouble());
	}
}

void LayerBaseProperties::sceneScalefluenceEditFinished()
{
	if (!InternalOperation && targetLayer){
		targetLayer->setSceneScaleFluence(sceneScale->text().toDouble());
	}
}

void LayerBaseProperties::filterTypeChange(int type_)
{
	if (!InternalOperation && targetLayer){
		targetLayer->setFilterType(type_);
	}
}

void LayerBaseProperties::EditFinishedDrawLayerRange()
{
	if (!InternalOperation && targetLayer){
		int col1 = startPosX->text().toInt();
		int row1 = startPosY->text().toInt();
		int col2 = endedPosX->text().toInt();
		int row2 = endedPosY->text().toInt();
		auto scene = static_cast<HelloWorld*>(g_scene);
		scene->getForeManager()->getLayerMoveRangeManage()->setDrawDataRect(targetLayer->getTagName(),
			Vec2(col1, row1), Vec2(col2, row2));
	}
}

void LayerBaseProperties::showTypeChange(int type_)
{
	if (!InternalOperation && targetLayer){
		if (type_ == 0){
			auto scene = static_cast<HelloWorld*>(g_scene);
			scene->getForeManager()->getLayerMoveRangeManage()->getLayerDrawNode(targetLayer->getTagName())->setVisible(false);
		}
		else{
			auto scene = static_cast<HelloWorld*>(g_scene);
			scene->getForeManager()->getLayerMoveRangeManage()->getLayerDrawNode(targetLayer->getTagName())->setVisible(true);
		}
	}
}

void LayerBaseProperties::setTargetLayer(ImageSpriteLayer* layer)
{
	targetLayer = layer;
	setWidgetValue();
}

void LayerBaseProperties::setWidgetValue()
{
	auto scene = static_cast<HelloWorld*>(g_scene);
	auto layerMoveRangeManage = scene->getForeManager()->getLayerMoveRangeManage();
	float moveX = targetLayer->getMoveScaleX();
	float moveY = targetLayer->getMoveScaleY();
	float scenes = targetLayer->getSceneScaleFluence();
	int type_ = targetLayer->getFilterType();
	moveScaleX->setText(QString::number(moveX));
	moveScaleY->setText(QString::number(moveY));
	sceneScale->setText(QString::number(scenes));
	startPosX->setText(QString::number(int(layerMoveRangeManage->getLayerStartPos(targetLayer->getTagName()).x)));
	startPosY->setText(QString::number(int(layerMoveRangeManage->getLayerStartPos(targetLayer->getTagName()).y)));
	endedPosX->setText(QString::number(int(layerMoveRangeManage->getLayerEndedPos(targetLayer->getTagName()).x)));
	endedPosY->setText(QString::number(int(layerMoveRangeManage->getLayerEndedPos(targetLayer->getTagName()).y)));
	filterType->setCurrentIndex(type_);
	showTypeComboBox->setCurrentIndex(layerMoveRangeManage->getLayerData(targetLayer->getTagName())->drawnode->isVisible());
}