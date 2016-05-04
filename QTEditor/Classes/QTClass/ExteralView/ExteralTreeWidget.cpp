﻿#include "ExteralTreeWidget.h"
#include "HelloWorldScene.h"
#include "GlobalConstant.h"
#include "QDir"

ExteralTreeWidget::ExteralTreeWidget(QWidget *parent)
	:ImageTreeWidget(parent, false)
{
	initTreeWidget();
	dialog = new ResModifyDirDialog();
	dialog->isResourceDialog(type__);
}

void ExteralTreeWidget::initTreeWidget()
{
	type__ = Image_Type::Entity;
	hasPrePacker = false;
	relativeDir = g_c_relaExteralDir;
	relativeListDir = g_c_rela_ExteralListDir;
	imagePath = FileUtils::getInstance()->getSearchPaths().at(0).c_str();
	listPath = imagePath;
	imagePath += relativeDir;
	listPath += relativeListDir;
}

ExteralTreeWidget::~ExteralTreeWidget()
{
}
