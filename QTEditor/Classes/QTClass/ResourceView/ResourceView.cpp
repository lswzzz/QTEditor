#include "ResourceView.h"
#include <QLayout>
#include "Global.h"
#include "FontChina.h"
#include "TexturePackerView.h"
#include "QDir"
#include "QMessageBox"

ResourceView::ResourceView(char* title, QSize size)
:BaseDockWidget(title, size)
{
	QVBoxLayout* vlayout = new QVBoxLayout(widget);
	QHBoxLayout* hlayout = new QHBoxLayout(widget);
	listWidget = new ImageListWidget();
	treeWidget = new ImageTreeWidget();
	treeWidget->setItemDelegate(new CustomItemDelegate());
	treeWidget->setHeaderHidden(true);
	pre_Packer = new QPushButton(FontChina::G2U("预打包"));
	showMode = new QComboBox();
	showMode->addItem(FontChina::G2U("文字模式"));
	showMode->addItem(FontChina::G2U("图标模式"));
	hlayout->addWidget(showMode);
	hlayout->addWidget(pre_Packer);
	vlayout->addLayout(hlayout);
	vlayout->addWidget(treeWidget);
	vlayout->addWidget(listWidget);
	listWidget->setVisible(false);
	m_mode = 0;
	widget->setLayout(vlayout);
	connect(pre_Packer, SIGNAL(clicked()), this, SLOT(on_prePacker_clicked()));
	connect(showMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setResShowMode(int)));
}

void ResourceView::changedResRef(std::string path, int count) {
	treeWidget->changedResRef(path, count);
}

void ResourceView::on_prePacker_clicked()
{
	QStringList list = treeWidget->getPackerResourceList();
	TexturePackerView* view = dynamic_cast<TexturePackerView*>(g_TextureView);
	view->prePacker(list);
}

void ResourceView::setResShowMode(int mode)
{
	switch (mode){
	case 0:
		listWidget->widgetVisible(false);
		listWidget->setVisible(false);
		treeWidget->setVisible(true);
		m_mode = mode;
		break;
	case 1:
		listWidget->widgetVisible(true);
		listWidget->setVisible(true);
		treeWidget->setVisible(false);
		m_mode = mode;
		break;
	}
}

ImageTreeWidget* ResourceView::getTreeWidget()
{
	return treeWidget;
}

ImageListWidget* ResourceView::getListWidget()
{
	return listWidget;
}

int ResourceView::getModel()
{
	return m_mode;
}

void ResourceView::setCurItem(int listpngindex)
{
	this->show();
	this->raise();
	if (listWidget->isVisible()){
		treeWidget->setCurRes(listpngindex);
		listWidget->update();
	}
	else{
		treeWidget->setCurRes(listpngindex);
	}
}

ResourceView::~ResourceView()
{
}
