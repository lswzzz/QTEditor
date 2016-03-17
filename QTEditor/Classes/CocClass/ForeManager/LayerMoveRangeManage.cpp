#include "LayerMoveRangeManage.h"
#include "Global.h"

LayerMoveRangeManage* LayerMoveRangeManage::create()
{
	LayerMoveRangeManage* ret = new LayerMoveRangeManage();
	if (ret->init()){
		return ret;
	}
	return nullptr;
}

void LayerMoveRangeManage::setGridData(int gridWidth, int gridHeight)
{
	this->gridWidth = gridWidth;
	this->gridHeight = gridHeight;
}

void LayerMoveRangeManage::addGrid(int count, int direction)
{
	std::map<std::string, DrawData*>::iterator iter;
	for (iter = drawMap_.begin(); iter != drawMap_.end(); iter++){
		auto data = iter->second;
		if (data->isDraw){
			switch (direction){
				//ио
			case 0:
				if (data->row1 > -1) {
					data->row1 += count;
					if (data->row1 < 0)
						data->row1 = 0;
				}
				if (data->row2 > -1) {
					data->row2 += count;
					if (data->row2 < 0){
						data->row2 = 0;
					}
				}
				setDrawDataRect(data);
				break;
				//об
			case 1:
				break;
				//вС
			case 2:
				if (data->col1 > -1) {
					data->col1 += count;
					if (data->col1 < 0)
						data->col1 = 0;
				}
				if (data->col2 > -1) {
					data->col2 += count;
					if (data->col2 < 0)
						data->col2 = 0;
				}
				setDrawDataRect(data);
				break;
				//ср
			case 3:

				break;
			}
		}
	}
}

Vec2 LayerMoveRangeManage::gridPosChangeToGLPosStart(Vec2 pos, bool isOpen)
{
	auto glview = Director::getInstance()->getOpenGLView();
	if (isOpen){
		return Vec2((int(pos.x))*gridWidth, g_CocosWindowInitSize.height() - (int(pos.y))*gridHeight);
	}
	else{
		//return Vec2((int(pos.x))*gridWidth, getNewWindowSize().height() - (int(pos.y))*gridHeight);
		float height = getNewWindowSize().height() - (int(pos.y))*gridHeight;
		if (g_CocosWindowInitSize.height() != getNewWindowSize().height()){
			height += getOldWindowSize().height() - getNewWindowSize().height();
		}
		return Vec2((int(pos.x))*gridWidth, height);
	}
}

Vec2 LayerMoveRangeManage::gridPosChangeToGLPosEnd(Vec2 pos, bool isOpen)
{
	auto glview = Director::getInstance()->getOpenGLView();
	if (isOpen){
		return Vec2((int(pos.x))*gridWidth, g_CocosWindowInitSize.height() - (int(pos.y))*gridHeight);
	}
	else{
		float height = getNewWindowSize().height() - (int(pos.y))*gridHeight;
		if (g_CocosWindowInitSize.height() != getNewWindowSize().height()){
			height += getOldWindowSize().height() - getNewWindowSize().height();
		}
		return Vec2((int(pos.x))*gridWidth, height);
		//return Vec2((int(pos.x)+1)*gridWidth, getNewWindowSize().height() - (int(pos.y)+1)*gridHeight);
	}
}

void LayerMoveRangeManage::addDrawData(std::string tagname)
{
	DrawNode* node = DrawNode::create();
	addChild(node);
	DrawData* data = new DrawData();
	data->row1 = -1;
	data->col1 = -1;
	data->row2 = -1;
	data->col2 = -1;
	data->isDraw = false;
	data->drawnode = node;
	data->drawnode->setVisible(false);
	drawMap_.insert(std::pair<std::string, DrawData*>(tagname, data));
}

void LayerMoveRangeManage::resetDrawData(std::string tagname, std::string newtagname)
{
	auto data = drawMap_[tagname];
	drawMap_.erase(tagname);
	drawMap_.insert(std::pair<std::string, DrawData*>(newtagname, data));
}

void LayerMoveRangeManage::setDrawDataRect(std::string tagname, Vec2 startPos, Vec2 endedPos, bool isOpen)
{
	auto data = drawMap_[tagname];
	auto node = data->drawnode;
	int col1 = startPos.x;
	int row1 = startPos.y;
	int col2 = endedPos.x;
	int row2 = endedPos.y;
	if (col1 > col2){
		int temp = col1;
		col1 = col2;
		col2 = temp;
	}
	if (row1 > row2){
		int temp = row1;
		row1 = row2;
		row2 = temp;
	}
	node->clear();
	if (col1 >= 0 && row1 >= 0 && col2 >= 0 && row2 >= 0){
		node->drawSolidRect(gridPosChangeToGLPosStart(Vec2(col1, row1), isOpen), gridPosChangeToGLPosEnd(Vec2(col2, row2), isOpen),
			Color4F(Color4B(0x9F, 0x79, 0xEE, 0x9F)));
	}
	data->row1 = row1;
	data->col1 = col1;
	data->row2 = row2;
	data->col2 = col2;
	data->isDraw = true;
}

void LayerMoveRangeManage::setDrawDataRect(DrawData* data)
{
	auto node = data->drawnode;
	node->clear();
	if (data->col1 >= 0 && data->row1 >= 0 && data->col2 >= 0 && data->row2 >= 0){
		node->drawSolidRect(gridPosChangeToGLPosStart(Vec2(data->col1, data->row1), false), gridPosChangeToGLPosEnd(Vec2(data->col2, data->row2), false),
			Color4F(Color4B(0x9F, 0x79, 0xEE, 0x9F)));
	}
}

void LayerMoveRangeManage::deleteDrawRect(std::string tagname)
{
	auto drawNode = drawMap_[tagname];
	drawMap_.erase(tagname);
	drawNode->drawnode->removeFromParent();
	delete drawNode;
}

Vec2 LayerMoveRangeManage::getLayerStartPos(std::string tagname)
{
	auto data = drawMap_[tagname];
	return Vec2(data->col1, data->row1);
}

Vec2 LayerMoveRangeManage::getLayerEndedPos(std::string tagname)
{
	auto data = drawMap_[tagname];
	return Vec2(data->col2, data->row2);
}

DrawNode* LayerMoveRangeManage::getLayerDrawNode(std::string tagname)
{
	return drawMap_[tagname]->drawnode;
}

LayerMoveRangeManage::DrawData* LayerMoveRangeManage::getLayerData(std::string tagname)
{
	return drawMap_[tagname];
}