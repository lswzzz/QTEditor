#pragma once
#include "BaseDockWidget.h"
#include <QScrollArea>
#include <QLayout>
#include <QPushButton>
#include "BaseProperties.h"
#include "MoveProperties.h"
#include "BoxProperties.h"
#include "ActionView.h"
#include "ImageSprite.h"
#include "EntityProperties.h"
#include "ImageSpriteLayer.h"
#include "LayerBaseProperties.h"
#include "MonsterProperties.h"
#include "MonsterDropProperties.h"
#include "CircleProperties.h"
#include "TriggerView.h"
#include "AIProperties.h"
#include "SimpleTriggerView.h"
#include "FilterProperties.h"

//可以显示精灵的信息也可以显示层的信息
class ControlView : public BaseDockWidget
{
	Q_OBJECT
public:
	ControlView(char* title, QSize size);
	~ControlView();
	QScrollArea* getScrollArea();
	bool addDockWidget(QDockWidget* widget);
	void addActionView();
	void addBaseView();
	void addBoxView();
	void addMoveView();
	void addEntityView();
	void addLayerBaseView();
	void addMonsterView();
	void addMonsterDropView();
	void addCircleView();
	void addTriggerView();
	void addSimpleTriggerView();
	void addAIView();
	void addFilterView();

	void addDock();
	void setTargetName(QString str);
	void setTargetPng(int index);
	void setTemplateFile(QString file, QString md5);
	void setTargetSprite(ImageSprite* sprite);
	void setTargetLayer(ImageSpriteLayer* layer);
	void setValueFromChildSprite(ImageSprite* sprite);
	void setValueFromLayer(ImageSpriteLayer* layer);
	//将模板文件的值赋予ControlView
	void setValueFromTemplateFile(QString file);
	void setInitState();
	void setValueForSpriteFromTemplateFile();
	void clearProperties();
	QString getTemplateFile();
	ActionView* getActionView();
	BaseProperties* getBaseProperties();
	MoveProperties* getMoveProperties();
	BoxProperties* getBoxProperties();
	EntityProperties* getEntityProperties();
	LayerBaseProperties* getLayerBaseProperties();
	MonsterProperties* getMonsterProperties();
	MonsterDropProperties* getMonsterDropProperties();
	CircleProperties* getCircleProperties();
	TriggerView* getTriggerProperties();
	AIProperties* getAIProperties();
	SimpleTriggerView* getSimplerTriggerProperties();
	FilterProperties* getFilterProperties();

	QString GenerateMD5();

	bool targetPropertiesIsSprite();
private slots:
	void on_saveButton_clicked();
	void on_modifyButton_clicked();
	void on_resetButton_clicked();
	void on_allSelectedButton_clicked();
	void on_locationButton_clicked();
protected:
	void setSelectedButton_state(bool state);
	enum showType{
		uiSprite,
		uiLayer,
	};
	void setUIViewShow(showType type__);
	void addValueToMap(QVariantMap& map);
	void __connect();
private:
	QLabel* contentSizeLabel;
	QLabel* targetPngLabel;
	QLabel* targetLabel;
	QLabel* templateLabel;
	QScrollArea* scrollArea;
	QPushButton* saveButton;
	QPushButton* modifyButton;
	QPushButton* resetButton;
	QPushButton* allSelectedButton;
	QPushButton* locationButton;
	std::vector<QDockWidget*> vec;

	ActionView* actionView;
	BaseProperties* baseProperties;
	MoveProperties* moveProperties;
	BoxProperties* boxProperties;
	EntityProperties* entityProperties;
	LayerBaseProperties* layerBaseProperties;
	MonsterProperties* monsterProperties;
	MonsterDropProperties* monsterDropProperties;
	CircleProperties* circleProperties;
	TriggerView* triggerProperties;
	AIProperties* aiProperties;
	SimpleTriggerView* simplerTrigger;
	FilterProperties* filterProperties;

	int targetListPngIndex;
	ImageSprite* targetSprite;
	ImageSpriteLayer* targetLayer;
	QString templateFile;
	QString templateMd5;

	showType show_type;
};

