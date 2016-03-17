#include "ExterImageListWidget.h"
#include "HelloWorldScene.h"
#include "LayerManagerWidget.h"
#include "Global.h"
#include "FontChina.h"
#include "QMessageBox"
#include "ExteralView.h"

ExterImageListWidget::ExterImageListWidget(QWidget *parent)
	:ImageListWidget(parent)
{
	type__ = Image_Type::Entity;
}


ExterImageListWidget::~ExterImageListWidget()
{
}

QTreeWidget* ExterImageListWidget::getTreeWidget()
{
	auto exteralView = static_cast<ExteralView*>(g_exteralView);
	return exteralView->getTreeWidget();
}
