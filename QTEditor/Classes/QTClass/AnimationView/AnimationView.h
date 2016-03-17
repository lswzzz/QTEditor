#pragma once
#include "BaseDockWidget.h"
#include "ResourceView.h"
#include "AnimationTreeWidget.h"

class AnimationView : public BaseDockWidget
{
	Q_OBJECT
public:
	AnimationView(char* title, QSize size);
	~AnimationView();
	AnimationTreeWidget* getTreeWidget();
	void changedResRef(std::string name, int count);
	void setCurAnimation(std::string animation);
private:

	AnimationTreeWidget* animationTreeWidget;
};

