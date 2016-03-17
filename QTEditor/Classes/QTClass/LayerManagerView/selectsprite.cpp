#include "selectsprite.h"
#include "ui_selectsprite.h"
#include "QLabel"
#include "FontChina.h"
#include "Global.h"
#include "LayerManagerWidget.h"
#include "HelloWorldScene.h"
#include "ControlView.h"
#include "ImgTransPath.h"
#include "ImgTransPath.h"

SelectSprite::SelectSprite(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectSprite)
{
    ui->setupUi(this);
	setFindList();
	_setLayout();
	_connect();
}

SelectSprite::~SelectSprite()
{
    delete ui;
}

void SelectSprite::setFindList()
{
	find_List << FontChina::G2U("标签") << FontChina::G2U("ID") << FontChina::G2U("模板") << FontChina::G2U("图片")
		<< FontChina::G2U("实体") << FontChina::G2U("怪物") << FontChina::G2U("线性移动") << FontChina::G2U("刚体")
		<< FontChina::G2U("AI") << FontChina::G2U("触发器") << FontChina::G2U("怪物掉落") << FontChina::G2U("动画");
}

void SelectSprite::_setLayout()
{
	find_Type = new QComboBox;
	for (int i = 0; i < find_List.size(); i++){
		find_Type->addItem(find_List.at(i));
	}
	sureButton = new QPushButton(FontChina::G2U("确定"));
	cancelButton = new QPushButton(FontChina::G2U("取消"));
	findButton = new QPushButton(FontChina::G2U("检索"));
	showTree = new QTreeWidget;
	showTree->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
	QStringList headers2;
	showTree->setHeaderLabels(headers2 << FontChina::G2U("tagname"));
	tagEdit = new QPlainTextEdit;
	tagEdit->setMaximumHeight(30);
	glayout = new QGridLayout;
	glayout->addWidget(new QLabel("Type:"), 0, 0, 1, 1);
	glayout->addWidget(find_Type, 0, 1, 1, 2);
	glayout->addWidget(tagEdit, 1, 0, 1, 3);
	glayout->addWidget(showTree, 2, 0, 1, 3);
	glayout->addWidget(new QLabel, 3, 0, 1, 3);
	glayout->addWidget(findButton, 4, 0, 1, 1);
	glayout->addWidget(sureButton, 4, 1, 1, 1);
	glayout->addWidget(cancelButton, 4, 2, 1, 1);
	setLayout(glayout);
	showTree->setVisible(false);
	glayout->setRowStretch(0, 1);
	glayout->setRowStretch(1, 1);
	glayout->setRowStretch(2, 0);
	glayout->setRowStretch(3, 100);
	glayout->setRowStretch(4, 1);
}

void SelectSprite::_connect()
{
	connect(find_Type, SIGNAL(currentIndexChanged(int)), this, SLOT(on_findType_change(int)));
	connect(sureButton, SIGNAL(clicked()), this, SLOT(on_sureButton_clicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(on_cancelButton_clicked()));
	connect(findButton, SIGNAL(clicked()), this, SLOT(on_findButton_clicked()));
	connect(showTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(imageitemClick(QTreeWidgetItem*, int)));
}

void SelectSprite::setTreeWidget(QTreeWidget* tree)
{
	layerTree = tree;
}

void SelectSprite::on_sureButton_clicked()
{
	operateByFind_Index();
	this->setVisible(false);
}

void SelectSprite::on_cancelButton_clicked()
{
	this->setVisible(false);
}

void SelectSprite::on_findButton_clicked()
{
	operateByFind_Index();
}

void SelectSprite::on_findType_change(int index)
{
	if (index == TAGNAME || index == ID){
		tagEdit->setVisible(true);
		showTree->setVisible(false);
		glayout->setRowStretch(0, 1);
		glayout->setRowStretch(1, 1);
		glayout->setRowStretch(2, 0);
		glayout->setRowStretch(3, 100);
		glayout->setRowStretch(4, 1);
	}
	else{
		switch (index){
		case TEMPLATE:
		case IMAGE:
			tagEdit->setVisible(true);
			showTree->setVisible(true);
			glayout->setRowStretch(0, 1);
			glayout->setRowStretch(1, 1);
			glayout->setRowStretch(2, 100);
			glayout->setRowStretch(3, 0);
			glayout->setRowStretch(4, 1);
			break;
		default:
			tagEdit->setVisible(false);
			showTree->setVisible(true);
			glayout->setRowStretch(0, 1);
			glayout->setRowStretch(1, 0);
			glayout->setRowStretch(2, 100);
			glayout->setRowStretch(3, 0);
			glayout->setRowStretch(4, 1);
			break;
		}
		void clearShowTree();
	}
}

void SelectSprite::clearShowTree()
{
	int count = showTree->topLevelItemCount();
	for (int i = 0; i < count; i++){
		showTree->takeTopLevelItem(0);
	}
}

void SelectSprite::operateByFind_Index()
{
	switch (find_Type->currentIndex()){
	case TAGNAME:
		selectByTagname();
		break;
	case ID:
		selectById();
		break;
	case TEMPLATE:
		treeTemplate(TEMPLATE);
		break;
	case IMAGE:
		treeTemplate(IMAGE);
		break;
	case ISENTITY:
		treeTemplate(ISENTITY);
		break;
	case ISMONSTER:
		treeTemplate(ISMONSTER);
		break;
	case HASLINEMOVE:
		treeTemplate(HASLINEMOVE);
		break;
	case HASAABBOX:
		treeTemplate(HASAABBOX);
		break;
	case HASAI:
		treeTemplate(HASAI);
		break;
	case HASTRIGGER:
		treeTemplate(HASTRIGGER);
		break;
	case HASMONSTERDROP:
		treeTemplate(HASMONSTERDROP);
		break;
	case HASANIMATION:
		treeTemplate(HASANIMATION);
		break;
	}
}

void SelectSprite::selectByTagname()
{
	auto tag = tagEdit->toPlainText();
	if (tag == ""){	return;	}
	auto layerWidget = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
	auto scene = dynamic_cast<HelloWorld*>(g_scene);
	auto count = layerTree->topLevelItemCount();
	for (int i = 0; i < count; i++){
		auto parentname = layerTree->topLevelItem(i)->text(1);
		auto sprite = scene->getSpriteChildFromForeManager(parentname.toStdString(), tag.toStdString());
		if (!sprite)continue;
		layerWidget->setCurrentSpriteChild(parentname.toStdString(), tag.toStdString());
	}
}

void SelectSprite::selectById()
{
	auto id = tagEdit->toPlainText().toInt();
	if (id == 0){ return; }
	auto layerWidget = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
	auto scene = dynamic_cast<HelloWorld*>(g_scene);
	auto vec = scene->getAllImageLayer();
	for (int i = 0; i < vec.size(); i++){
		auto layer = vec.at(i);
		auto spriteVec = layer->getAllSpriteNode();
		for (int k = 0; k < spriteVec.size(); k++){
			auto sprite = dynamic_cast<ImageSprite*>(spriteVec.at(k));
			if (sprite->getId() == id){
				auto parentname = layer->getTagName();
				auto childname = sprite->getTagName();
				layerWidget->setCurrentSpriteChild(parentname, childname);
			}
		}
	}
}

void SelectSprite::treeTemplate(index_Type type_)
{
	clearShowTree();
	auto scene = dynamic_cast<HelloWorld*>(g_scene);
	auto vec = scene->getAllImageLayer();
	for (int i = 0; i < vec.size(); i++){
		auto layer = vec.at(i);
		QString parentname = layer->getTagName().c_str();
		auto parentitem = AddToRootShowTree(parentname);
		auto spriteVec = layer->getAllSpriteNode();
		for (int k = 0; k < spriteVec.size(); k++){
			auto sprite = dynamic_cast<ImageSprite*>(spriteVec.at(k));
			switch (type_){
			case TAGNAME:
				break;
			case ID:
				break;
			case TEMPLATE:
				selectByTemplate(parentitem, sprite);
				break;
			case IMAGE:
				selectByImage(parentitem, sprite);
				break;
			case ISENTITY:
				selectByEntity(parentitem, sprite);
				break;
			case ISMONSTER:
				selectByMonster(parentitem, sprite);
				break;
			case HASLINEMOVE:
				selectByLineMove(parentitem, sprite);
				break;
			case HASAABBOX:
				selectByAABBox(parentitem, sprite);
				break;
			case HASAI:
				selectByAI(parentitem, sprite);
				break;
			case HASTRIGGER:
				selectByTrigger(parentitem, sprite);
				break;
			case HASMONSTERDROP:
				selectByMonsterDrop(parentitem, sprite);
				break;
			case HASANIMATION:
				selectByAnimation(parentitem, sprite);
				break;
			}
		}
	}
	clearNoChildRoot();
}
 
void SelectSprite::selectByTemplate(QTreeWidgetItem* parentitem, ImageSprite* sprite)
{
	auto templat = tagEdit->toPlainText();
	QString spriteTem = sprite->getTemplateFile().c_str();
	spriteTem = spriteTem.section("/", -1);
	if (spriteTem == templat){
		auto item = new QTreeWidgetItem;
		item->setText(0, sprite->getTagName().c_str());
		parentitem->addChild(item);
	}
}

void SelectSprite::selectByImage(QTreeWidgetItem* parentitem, ImageSprite* sprite)
{
	auto image_ = tagEdit->toPlainText();
	auto ss = ImgTransPath::getListPngName(sprite);
	if (ss == image_){
		auto item = new QTreeWidgetItem;
		item->setText(0, sprite->getTagName().c_str());
		parentitem->addChild(item);
	}
}

void SelectSprite::selectByEntity(QTreeWidgetItem* parentitem, ImageSprite* sprite)
{
	if (sprite->isHasEntityProperties()){
		auto item = new QTreeWidgetItem;
		item->setText(0, sprite->getTagName().c_str());
		parentitem->addChild(item);
	}
}

void SelectSprite::selectByMonster(QTreeWidgetItem* parentitem, ImageSprite* sprite)
{
	if (sprite->isHasMonsterProperties()){
		auto item = new QTreeWidgetItem;
		item->setText(0, sprite->getTagName().c_str());
		parentitem->addChild(item);
	}
}

void SelectSprite::selectByLineMove(QTreeWidgetItem* parentitem, ImageSprite* sprite)
{
	if (sprite->isHasMoveProperties()){
		auto item = new QTreeWidgetItem;
		item->setText(0, sprite->getTagName().c_str());
		parentitem->addChild(item);
	}
}

void SelectSprite::selectByAABBox(QTreeWidgetItem* parentitem, ImageSprite* sprite)
{
	if (sprite->isHasBoxProperties()){
		auto item = new QTreeWidgetItem;
		item->setText(0, sprite->getTagName().c_str());
		parentitem->addChild(item);
	}
}

void SelectSprite::selectByAI(QTreeWidgetItem* parentitem, ImageSprite* sprite)
{
	if (sprite->isHasAIProperties()){
		auto item = new QTreeWidgetItem;
		item->setText(0, sprite->getTagName().c_str());
		parentitem->addChild(item);
	}
}

void SelectSprite::selectByTrigger(QTreeWidgetItem* parentitem, ImageSprite* sprite)
{
	if (sprite->isHasSimplerTrigger()){
		auto item = new QTreeWidgetItem;
		item->setText(0, sprite->getTagName().c_str());
		parentitem->addChild(item);
	}
}

void SelectSprite::selectByMonsterDrop(QTreeWidgetItem* parentitem, ImageSprite* sprite)
{
	if (sprite->isHasMonsterDropProperties()){
		auto item = new QTreeWidgetItem;
		item->setText(0, sprite->getTagName().c_str());
		parentitem->addChild(item);
	}
}

void SelectSprite::selectByAnimation(QTreeWidgetItem* parentitem, ImageSprite* sprite)
{
	if (sprite->isHasAnimation()){
		auto item = new QTreeWidgetItem;
		item->setText(0, sprite->getTagName().c_str());
		parentitem->addChild(item);
	}
}

QTreeWidgetItem* SelectSprite::AddToRootShowTree(QString parentname)
{
	auto item = new QTreeWidgetItem;
	item->setText(0, parentname);
	showTree->addTopLevelItem(item);
	return item;
}

void SelectSprite::clearNoChildRoot()
{
	auto count = showTree->topLevelItemCount();
	QVector<QTreeWidgetItem*> vec;
	for (int i = 0; i < count; i++){
		auto item = showTree->topLevelItem(i);
		if (item->childCount() == 0){
			vec.push_back(item);
		}
	}
	for (int i = 0; i < vec.size(); i++){
		delete vec.at(i);
	}
}

void SelectSprite::imageitemClick(QTreeWidgetItem* item, int columns)
{
	if (item->parent()){
		auto layerWidget = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
		auto parentname = item->parent()->text(0);
		auto childname = item->text(0);
		layerWidget->setCurrentSpriteChild(parentname.toStdString(), childname.toStdString());
		HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
		scene->showNodeEdge(parentname.toStdString(), childname.toStdString(), false);
		ImageSprite* sprite = scene->getSpriteChildFromForeManager(parentname.toStdString(), childname.toStdString());
		auto controlView = dynamic_cast<ControlView*>(g_ControlView);
		controlView->setValueFromChildSprite(sprite);
	}
}