#ifndef SPRITEITEMMANAGERDIALOG_H
#define SPRITEITEMMANAGERDIALOG_H

#include <QDialog>
#include <qtreewidget.h>
#include "QPushButton"

namespace Ui {
class SpriteItemManagerDialog;
}

class SpriteItemManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SpriteItemManagerDialog(QWidget *parent = 0);
    ~SpriteItemManagerDialog();
	void addShowLayer(QList<QTreeWidgetItem*> list);
	void addMoveSprite(QList<QTreeWidgetItem*> list);
protected:
	void _setLayout();
	void _connect();
private slots:
	void on_sureButton_clicked();
	void on_cancelButton_clicked();
private:
	void removeAllItem();
	void moveSprites();
private:
    Ui::SpriteItemManagerDialog *ui;
	QList<QTreeWidgetItem*> spriteList;
	QTreeWidget* layerWidget;
	QPushButton* sureButton;
	QPushButton* cancelButton;
};

#endif // SPRITEITEMMANAGERDIALOG_H
