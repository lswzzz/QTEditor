#include "AnimationView.h"
#include <QLayout>

AnimationView::AnimationView(char* title, QSize size)
	:BaseDockWidget(title, size)
{
	QVBoxLayout* vlayout = new QVBoxLayout(widget);
	animationTreeWidget = new AnimationTreeWidget;
	animationTreeWidget->setHeaderHidden(true);
	animationTreeWidget->setItemDelegate(new CustomItemDelegate());
	vlayout->addWidget(animationTreeWidget);
	widget->setLayout(vlayout);
}


AnimationView::~AnimationView()
{
}

AnimationTreeWidget* AnimationView::getTreeWidget()
{
	return animationTreeWidget;
}

void AnimationView::changedResRef(std::string name, int count)
{
	animationTreeWidget->changedResRef(name, count);
}

void AnimationView::setCurAnimation(std::string animation)
{
	this->show();
	this->raise();
	animationTreeWidget->setCurrentItem(animationTreeWidget->getAnimationItem(animation.c_str()));
}