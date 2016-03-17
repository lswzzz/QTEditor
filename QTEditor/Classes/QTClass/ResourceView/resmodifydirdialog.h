#ifndef RESMODIFYDIRDIALOG_H
#define RESMODIFYDIRDIALOG_H

#include <QDialog>
#include "QTreeWidget"
#include "QPushButton"
#include "ResModifyTreeWidget.h"
#include "Global.h"

namespace Ui {
class ResModifyDirDialog;
}

class ResModifyDirDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResModifyDirDialog(QWidget *parent = 0);
    ~ResModifyDirDialog();
	void isResourceDialog(Image_Type type_);
	void showDir();

protected:
	void _setLayout();
	void _connect();
	QTreeWidgetItem* createNewTreeItem(QTreeWidgetItem* itemData);
	void createChildItem(QTreeWidgetItem* itemParent, QTreeWidgetItem* dirParent);
private slots:
	void on_sureButton_clicked();
	void on_cancelButton_clicked();
private:
	void removeAllItem();
	void removeChildItem(QTreeWidgetItem* item);
private:
    Ui::ResModifyDirDialog *ui;
	QMap<QTreeWidgetItem*, QTreeWidgetItem*> _map;
	ResModifyTreeWidget* treeWidget;
	QPushButton* sureButton;
	QPushButton* cancelButton;
	Image_Type res_Type;
};

#endif // RESMODIFYDIRDIALOG_H
