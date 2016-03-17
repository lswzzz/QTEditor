#include "ConfigView.h"
#include "HelloWorldScene.h"
#include "Global.h"
#include "QLayout"
#include "FontChina.h"
#include "QLabel"

ConfigView::ConfigView(char* title, QSize size)
:BaseDockWidget(title, size)
{
	sceneScale = new QLineEdit();
	minScale = new QLineEdit();
	maxScale = new QLineEdit();
	setOption = new QPushButton(FontChina::G2U("确定"));
	optionBox = new QComboBox;
	optionBox->addItem(FontChina::G2U("显示图片"));
	optionBox->addItem(FontChina::G2U("显示网格"));
	optionBox->addItem(FontChina::G2U("隐藏图片"));
	optionBox->addItem(FontChina::G2U("隐藏网格"));
	optionBox->addItem(FontChina::G2U("显示精灵边缘"));
	optionBox->addItem(FontChina::G2U("隐藏精灵边缘"));
	resetScenePosButton = new QPushButton(FontChina::G2U("还原场景位置"));
	showWidth = new QLineEdit();
	showHeight = new QLineEdit();
	rColor = new QLineEdit();
	gColor = new QLineEdit();
	bColor = new QLineEdit();
	setColorButton = new QPushButton(FontChina::G2U("修改背景"));
	gridRColor = new QLineEdit();
	gridGColor = new QLineEdit;
	gridBColor = new QLineEdit;
	gridAColor = new QLineEdit;
	setGridColorButton = new QPushButton(FontChina::G2U("确定"));
	QLabel* label = new QLabel(FontChina::G2U("最小缩放值"));
	QLabel* label2 = new QLabel(FontChina::G2U("最大缩放值"));
	QLabel* label3 = new QLabel(FontChina::G2U("场景缩放大小"));
	QLabel* label4 = new QLabel(FontChina::G2U("选项设置"));
	QLabel* label5 = new QLabel(FontChina::G2U("显示宽度"));
	QLabel* label6 = new QLabel(FontChina::G2U("显示高度"));
	QLabel* label7 = new QLabel(FontChina::G2U("bgR"));
	QLabel* label8 = new QLabel(FontChina::G2U("bgG"));
	QLabel* label9 = new QLabel(FontChina::G2U("bgB"));
	QLabel* label10 = new QLabel(FontChina::G2U("gridR"));
	QLabel* label11 = new QLabel(FontChina::G2U("gridG"));
	QLabel* label12 = new QLabel(FontChina::G2U("gridB"));
	QLabel* label13 = new QLabel(FontChina::G2U("gridA"));
	QGridLayout* gridLayout = new QGridLayout;
	gridLayout->addWidget(label, 0, 0);
	gridLayout->addWidget(minScale, 0, 1);
	gridLayout->addWidget(label2, 0, 2);
	gridLayout->addWidget(maxScale, 0, 3);
	gridLayout->addWidget(label3, 1, 0);
	gridLayout->addWidget(sceneScale, 1, 1);
	gridLayout->addWidget(resetScenePosButton, 1, 2);
	gridLayout->addWidget(label4, 2, 0);
	gridLayout->addWidget(optionBox, 2, 1);
	gridLayout->addWidget(setOption, 2, 2);
	gridLayout->addWidget(label5, 3, 0);
	gridLayout->addWidget(showWidth, 3, 1);
	gridLayout->addWidget(label6, 3, 2);
	gridLayout->addWidget(showHeight, 3, 3);
	QHBoxLayout* layout = new QHBoxLayout();
	layout->addWidget(label7);
	layout->addWidget(rColor);
	layout->addWidget(label8);
	layout->addWidget(gColor);
	layout->addWidget(label9);
	layout->addWidget(bColor);
	layout->addWidget(setColorButton);
	QHBoxLayout* layout2 = new QHBoxLayout;
	layout2->addWidget(label10);
	layout2->addWidget(gridRColor);
	layout2->addWidget(label11);
	layout2->addWidget(gridGColor);
	layout2->addWidget(label12);
	layout2->addWidget(gridBColor);
	layout2->addWidget(label13);
	layout2->addWidget(gridAColor);
	layout2->addWidget(setGridColorButton);
	QHBoxLayout* layout3 = new QHBoxLayout();
	isLayerStructure = new QCheckBox(FontChina::G2U("多配置文件"));
	isLayerStructure->setCheckState(Qt::CheckState::Unchecked);
	layout3->addWidget(isLayerStructure);
	gridLayout->addLayout(layout, 4, 0, 1, 4);
	gridLayout->addLayout(layout2, 5, 0, 1, 4);
	gridLayout->addLayout(layout3, 6, 0, 1, 4);
	gridLayout->setRowStretch(0, 1);
	gridLayout->setRowStretch(1, 1);
	gridLayout->setRowStretch(2, 1);
	gridLayout->setRowStretch(3, 1);
	gridLayout->setRowStretch(4, 1);
	gridLayout->setRowStretch(5, 1);
	gridLayout->setRowStretch(6, 1);
	gridLayout->addWidget(new QLabel, 7, 0, 1, 4);
	gridLayout->setRowStretch(7, 10);
	QVBoxLayout* vlayout = new QVBoxLayout;
	vlayout->addLayout(gridLayout);
	widget->setLayout(vlayout);
	__connect();
}

ConfigView::~ConfigView()
{
}

void ConfigView::__connect()
{
	connect(sceneScale, SIGNAL(editingFinished()), this, SLOT(scaleTextRedo()));
	connect(minScale, SIGNAL(editingFinished()), this, SLOT(minScaleRedo()));
	connect(maxScale, SIGNAL(editingFinished()), this, SLOT(maxScaleRedo()));
	connect(setOption, SIGNAL(clicked()), this, SLOT(on_setOptionButton_clicked()));
	connect(showWidth, SIGNAL(editingFinished()), this, SLOT(showWidthRedo()));
	connect(showHeight, SIGNAL(editingFinished()), this, SLOT(showHeightRedo()));
	connect(resetScenePosButton, SIGNAL(clicked()), this, SLOT(resetScenePos_clicked()));
	connect(setColorButton, SIGNAL(clicked()), this, SLOT(setColorButton_clicked()));
	connect(setGridColorButton, SIGNAL(clicked()), this, SLOT(setGridColorButton_clicked()));
}

void ConfigView::scaleTextRedo()
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	double scale = sceneScale->text().toDouble();
	if (scale != 0.0f && scale > 0.0f){
		float oldscele = scene->getScale();
		scene->setScale(scale);
		scene->setPosition(scene->getPosition()*(scene->getScale() / oldscele));
	}
}

void ConfigView::minScaleRedo()
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	scene->minScale = minScale->text().toFloat();
}

void ConfigView::maxScaleRedo()
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	scene->maxScale = maxScale->text().toFloat();
}

void ConfigView::on_setOptionButton_clicked()
{
	int index = optionBox->currentIndex();
	HelloWorld* scene = (HelloWorld*)g_scene;
	scene->setOptionShowOrHide(index);
}

void ConfigView::showWidthRedo()
{
	HelloWorld* scene = (HelloWorld*)g_scene;
	scene->showWidth = showWidth->text().toInt();
}

void ConfigView::showHeightRedo()
{
	HelloWorld* scene = (HelloWorld*)g_scene;
	scene->showHeight = showHeight->text().toInt();
}

void ConfigView::setScaleTextValue(float scale)
{
	sceneScale->setText(QString::number(scale));
}

bool ConfigView::getLayerStructureState()
{
	if (isLayerStructure->checkState() == Qt::CheckState::Unchecked){
		return false;
	}
	else{
		return true;
	}
}

void ConfigView::setLayerStructureState(bool state)
{
	if (state){
		isLayerStructure->setCheckState(Qt::CheckState::Checked);
	}
	else{
		isLayerStructure->setCheckState(Qt::CheckState::Unchecked);
	}
}

void ConfigView::resetScenePos_clicked()
{
	auto scene = dynamic_cast<HelloWorld*>(g_scene);
	scene->setPosition(Point(0, 0));
}

void ConfigView::setColorButton_clicked()
{
	auto scene = dynamic_cast<HelloWorld*>(g_scene);
	int r = rColor->text().toInt();
	int g = gColor->text().toInt();
	int b = bColor->text().toInt();
	scene->setLayerColor(Color3B(r, g, b));
}

void ConfigView::setGridColorButton_clicked()
{
	auto scene = dynamic_cast<HelloWorld*>(g_scene);
	int r = gridRColor->text().toInt();
	int g = gridGColor->text().toInt();
	int b = gridBColor->text().toInt();
	int a = gridAColor->text().toInt();
	scene->setGridColor(Color4B(r, g, b, a));
}

void ConfigView::setGridColorR(int r)
{
	gridRColor->setText(QString::number(r));
}

void ConfigView::setGridColorG(int g)
{
	gridGColor->setText(QString::number(g));
}

void ConfigView::setGridColorB(int b)
{
	gridBColor->setText(QString::number(b));
}

void ConfigView::setGridColorA(int a)
{
	gridAColor->setText(QString::number(a));
}

void ConfigView::setColorR(int r)
{
	rColor->setText(QString::number(r));
}

void ConfigView::setColorG(int g)
{
	gColor->setText(QString::number(g));
}

void ConfigView::setColorB(int b)
{
	bColor->setText(QString::number(b));
}
