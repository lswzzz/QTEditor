#ifndef SELECTSPRITE_H
#define SELECTSPRITE_H

#include <QDialog>
#include "QComboBox.h"
#include "QPushButton"
#include "QPlainTextEdit"
#include "QTreeWidget"
#include "ImageSprite.h"
#include "QLayout"

namespace Ui {
class SelectSprite;
}

class SelectSprite : public QDialog
{
    Q_OBJECT

public:
    explicit SelectSprite(QWidget *parent = 0);
    ~SelectSprite();
	void setTreeWidget(QTreeWidget* tree);
private slots:
	void on_sureButton_clicked();
	void on_cancelButton_clicked();
	void on_findButton_clicked();
	void on_findType_change(int index);
	void imageitemClick(QTreeWidgetItem* item, int columns);
private:
	void _setLayout();
	void _connect();
	void setFindList();

	void clearShowTree();
	void operateByFind_Index();
	void selectByTagname();
	void selectById();
	void selectByTemplate(QTreeWidgetItem* parentitem, ImageSprite* sprite);
	void selectByImage(QTreeWidgetItem* parentitem, ImageSprite* sprite);
	void selectByEntity(QTreeWidgetItem* parentitem, ImageSprite* sprite);
	void selectByMonster(QTreeWidgetItem* parentitem, ImageSprite* sprite);
	void selectByLineMove(QTreeWidgetItem* parentitem, ImageSprite* sprite);
	void selectByAABBox(QTreeWidgetItem* parentitem, ImageSprite* sprite);
	void selectByAI(QTreeWidgetItem* parentitem, ImageSprite* sprite);
	void selectByTrigger(QTreeWidgetItem* parentitem, ImageSprite* sprite);
	void selectByMonsterDrop(QTreeWidgetItem* parentitem, ImageSprite* sprite);
	void selectByAnimation(QTreeWidgetItem* parentitem, ImageSprite* sprite);
	enum index_Type{
		TAGNAME,
		ID,
		TEMPLATE,
		IMAGE,
		ISENTITY,
		ISMONSTER,
		HASLINEMOVE,
		HASAABBOX,
		HASAI,
		HASTRIGGER,
		HASMONSTERDROP,
		HASANIMATION,
	};
	void treeTemplate(index_Type type_);
	QTreeWidgetItem* AddToRootShowTree(QString parentname);
	void clearNoChildRoot();
private:
    Ui::SelectSprite *ui;
	QComboBox* find_Type;
	QPushButton* sureButton;
	QPushButton* cancelButton;
	QPushButton* findButton;
	QPlainTextEdit *tagEdit;
	QTreeWidget* layerTree;
	QTreeWidget* showTree;
	QStringList find_List;
	QGridLayout* glayout;
	
};

#endif // SELECTSPRITE_H
