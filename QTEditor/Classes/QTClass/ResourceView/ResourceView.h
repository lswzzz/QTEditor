#pragma once
#include "BaseDockWidget.h"
#include <qpushbutton.h>
#include "ImageListWidget.h"
#include <qpushbutton.h>
#include <QComboBox>
#include "ImageTreeWidget.h"
#include "QItemDelegate.h"

class CustomItemDelegate : public QItemDelegate {
	Q_OBJECT
public:
	CustomItemDelegate() {
	}
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
		QStyleOptionViewItem  viewOption(option);
		viewOption.palette.setColor(QPalette::HighlightedText, index.data(Qt::ForegroundRole).value<QColor>());
		viewOption.palette.setColor(QPalette::Highlight, QColor(230, 230, 230, 255));
		QItemDelegate::paint(painter, viewOption, index);
	}
};


class ResourceView : public BaseDockWidget
{
	Q_OBJECT
public:
	ResourceView(char* title, QSize size);
	~ResourceView();
	ImageTreeWidget* getTreeWidget();
	ImageListWidget* getListWidget();
	void changedResRef(std::string path, int count);

	int getModel();
	void setCurItem(int listpngindex);
private slots:
	void on_prePacker_clicked();
	void setResShowMode(int mode);
	
private:
	
private:
	QPushButton* pre_Packer;
	ImageListWidget* listWidget;
	ImageTreeWidget* treeWidget;
	QComboBox* showMode;
	int m_mode;
};

