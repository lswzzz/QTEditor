#include "BMFontLayer.h"
#include "Global.h"

BMFontLayer::BMFontLayer(int row, int col)
{
	this->row = row;
	this->col = col;
	vec = new LabelAtlas**[row];
	for (int i = 0; i < row; i++){
		vec[i] = new LabelAtlas*[i];
	}
	for (int i = 0; i < row; i++){
		for (int j = 0; j < col; j++){
			LabelAtlas* label = LabelAtlas::create("1", "fonts/tuffy_bold_italic-charmap.plist");
			label->setScale(0.15);
			this->addChild(label, 100);
			vec[i][j] = label;
			label->setVisible(false);
		}
	}
}


BMFontLayer::~BMFontLayer()
{
	for (int i = 0; i < row; i++){
		for (int j = 0; j < col; j++){
			vec[i][j]->removeFromParent();
			vec[i][j] = NULL;
		}
	}
	for (int i = 0; i < row; i++){
		delete vec[i];
	}
	delete vec;
}

bool BMFontLayer::init()
{
	Layer::init();
	return true;
}

void BMFontLayer::addLabel(int tag, Point pos)
{
	/*har str[32];
	sprintf(str, "%d", tag);
	LabelAtlas* label = LabelAtlas::create(str, "fonts/tuffy_bold_italic-charmap.plist");
	label->setScale(0.15);
	this->addChild(label, 100);
	label->setPosition(pos.x, pos.y);
	label->setAnchorPoint(ccp(0.5, 0.5));
	int i = tag / row;
	int j = tag % row;
	vec[i][j] = label;*/
	int i = tag / row;
	int j = tag % row;
	auto label = vec[i][j];
	char str[32];
	sprintf(str, "%d", tag);
	label->setString(str);
	label->setPosition(pos.x, pos.y);
	label->setVisible(true);
}

void BMFontLayer::delLabel(int tag)
{
	int i = tag / row;
	int j = tag % row;
	auto label = vec[i][j];
	label->setVisible(false);
}