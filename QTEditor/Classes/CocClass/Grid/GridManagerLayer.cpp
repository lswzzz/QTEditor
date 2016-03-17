#include "GridManagerLayer.h"
#include "Global.h"

static int gridWidth;
static int gridHeight;

enum GridZRoder
{
	GridZRoder_bg,
	GridZRoder_grid,
	GridZRoder_touch
};

GridManagerLayer::GridManagerLayer()
{
	addSprite = NULL;
	gridSpriteLayer = NULL;
	deleteSprite = NULL;
	selectDirectionSprite = NULL;
	touchSprite = NULL;
	drawnode = NULL;
	multiselectBlock = NULL;
	is_Shift_Key = false;
	BBSize = 0;
	BBvec = NULL;
	gridColor = Color4B(255, 255, 255, 125);
	row = 0;
	col = 0;
}

GridManagerLayer* GridManagerLayer::create()
{
	auto ret = new GridManagerLayer;
	if (ret && ret->init()){
		return ret;
	}
	return nullptr;
}

bool GridManagerLayer::init()
{
	if (!Layer::init()){
		return false;
	}
	addSprite = AddSprite::create();
	addChild(addSprite, GridZRoder_touch);
	deleteSprite = DeleteSprite::create();
	addChild(deleteSprite, GridZRoder_touch);
	selectDirectionSprite = SelectDirectionSprite::create();
	addChild(selectDirectionSprite, GridZRoder_touch);
	touchSprite = TouchSprite::create();
	addChild(touchSprite, GridZRoder_touch);
	touchSprite->setAnchorPoint(ccp(0, 0));
	multiselectBlock = MultiselectBlock::create();
	addChild(multiselectBlock, GridZRoder_touch);
	return true;
}

GridManagerLayer::~GridManagerLayer()
{
	if (addSprite){
		addSprite->removeFromParent();
		addSprite->release();
	}
	if (gridSpriteLayer){
		gridSpriteLayer->removeFromParent();
		gridSpriteLayer->release();
	}
	if (deleteSprite){
		deleteSprite->removeFromParent();
		deleteSprite->release();
	}
	if (selectDirectionSprite){
		selectDirectionSprite->removeFromParent();
		selectDirectionSprite->release();
	}
	if (touchSprite){
		touchSprite->removeFromParent();
		touchSprite->release();
	}
	if (drawnode){
		drawnode->removeFromParent();
	}
	if (multiselectBlock){
		multiselectBlock->removeFromParent();
		multiselectBlock->release();
	}
}

Point GridManagerLayer::setItalicPosAndTouchPos(int type)
{
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	float width, height;
	if (type == 1){
		width = startPos.x;
		height = glview->getFrameSize().height - startPos.y;
	}
	else{
		width = endedPos.x;
		height = glview->getFrameSize().height - endedPos.y;
	}
	int col = width / gridWidth;
	int row = height / gridHeight;
	float width1 = col *  gridWidth;
	float height1 = row * gridHeight;
	float width2 = width1 + gridWidth;
	float height2 = height1 + gridHeight;
	Point pos = Point(width, height);
	Point setPos;
	if (type == 1){
		touchSprite->setType(0);
		setPos = startPos;
	}
	else{
		setPos = endedPos;
	}
	//如果按下了Shift键的话就必须吸附
	if (is_Shift_Key){
		if ((pos.x - width1 <= gridWidth / 2 && pos.x - width1 > 0.0) || (width1 - pos.x <= gridWidth / 2 && width1 - pos.x > 0.0)){
			if (type == 1){
				ItalicPos.x = width1;
				touchSprite->setType(1);
			}
			setPos.x = width1;
		}
		else if ((pos.x - width2 <= gridWidth / 2 && pos.x - width2 > 0.0) || (width2 - pos.x <= gridWidth / 2 && width2 - pos.x > 0.0)){
			if (type == 1){
				ItalicPos.x = width2;
				touchSprite->setType(1);
			}
			setPos.x = width2;
		}
		if ((pos.y - height1 <= gridHeight / 2 && pos.y - height1 > 0.0f) || (height1 - pos.y <= gridHeight / 2 && height1 - pos.y > 0.0)){
			if (type == 1){
				ItalicPos.y = glview->getFrameSize().height - height1;
				touchSprite->setType(1);
			}
			setPos.y = glview->getFrameSize().height - height1;
		}
		else if ((pos.y - height2 <= gridHeight / 2 && pos.y - height2 > 0.0f) || (height2 - pos.y <= gridHeight / 2 && height2 - pos.y > 0.0)){
			if (type == 1){
				ItalicPos.y = glview->getFrameSize().height - height2;
				touchSprite->setType(1);
			}
			setPos.y = glview->getFrameSize().height - height2;
		}
	}
	else{
		if ((pos.x - width1 <= 7.0f && pos.x - width1 > 0.0) || (width1 - pos.x <= 7.0f && width1 - pos.x > 0.0)){
			if (type == 1){
				ItalicPos.x = width1;
				touchSprite->setType(1);
			}
			setPos.x = width1;
		}
		else if ((pos.x - width2 <= 7.0f && pos.x - width2 > 0.0) || (width2 - pos.x <= 7.0f && width2 - pos.x > 0.0)){
			if (type == 1){
				ItalicPos.x = width2;
				touchSprite->setType(1);
			}
			setPos.x = width2;
		}
		if ((pos.y - height1 <= 7.0f && pos.y - height1 > 0.0f) || (height1 - pos.y <= 7.0f && height1 - pos.y > 0.0)){
			if (type == 1){
				ItalicPos.y = glview->getFrameSize().height - height1;
				touchSprite->setType(1);
			}
			setPos.y = glview->getFrameSize().height - height1;
		}
		else if ((pos.y - height2 <= 7.0f && pos.y - height2 > 0.0f) || (height2 - pos.y <= 7.0f && height2 - pos.y > 0.0)){
			if (type == 1){
				ItalicPos.y = glview->getFrameSize().height - height2;
				touchSprite->setType(1);
			}
			setPos.y = glview->getFrameSize().height - height2;
		}
	}
	if (type == 1)
		touchSprite->setStartPos(setPos);
	else
		touchSprite->setEndedPos(setPos);
	return setPos;
}

void GridManagerLayer::setSelectDirectionSpriteStartPos(Point pos)
{
	selectDirectionSprite->setStartPos(pos);
}

void GridManagerLayer::setSelectDirectionSpriteMovedPos(Point pos)
{
	selectDirectionSprite->setMovedPos(pos);
}

void GridManagerLayer::setSelectDirectionSpriteEndedPos(Point pos)
{
	selectDirectionSprite->setEndedPos(pos);
	std::vector<Point> poss = selectDirectionSprite->getRange(gridWidth, gridHeight);
	int size = poss.size();
	for (int i = 0; i < size; i++){
		int x = poss.at(i).x;
		int y = poss.at(i).y;
		if (x >= this->col || y >= this->row || x < 0 || y < 0)
			continue;
		if (BBvec && BBvec[y][x] != NULL && BBvec[y][x]->getType() == Type_Box){
			auto block = dynamic_cast<BoxBody*>(BBvec[y][x]);
			block->setWay((One_Way)selectDirectionSprite->getType());
			block->DrawDirection();
		}
	}
}

void GridManagerLayer::setAddSpriteStartPos(Point pos)
{
	addSprite->setStartPos(pos);
}

void GridManagerLayer::setAddSpriteMovedPos(Point pos)
{
	addSprite->setMovedPos(pos);
}

void GridManagerLayer::setAddSpriteEndedPos(Point pos)
{
	auto glview = Director::getInstance()->getOpenGLView();
	addSprite->setEndedPos(pos);
	std::vector<Point> poss = addSprite->getRange(gridWidth, gridHeight);
	int size = poss.size();
	for (int i = 0; i < size; i++){
		int x = poss.at(i).x;
		int y = poss.at(i).y;
		if (x < 0 || y < 0 || x > this->col - 1 || y > this->row - 1)
			continue;
		if (BBvec && BBvec[y][x] == NULL){
			auto block = BoxBody::create("gridBg.png", gridWidth, gridHeight);
			auto pos = Point(x*gridWidth + gridWidth / 2, glview->getFrameSize().height - y*gridHeight - gridHeight / 2);
			if (g_CocosWindowInitSize.height() != getNewWindowSize().height()){
				pos.y += getOldWindowSize().height() - getNewWindowSize().height();
			}
			block->setPosition(pos );
			this->gridSpriteLayer->addChild(block);
			block->setTag(y*col + x);
			block->setOpacity(80);
			block->setType(Type_Box);
			BBvec[y][x] = block;
			BBSize += 1;
			block->DrawGrid();
		}
	}
}

void GridManagerLayer::setDeleteSpriteStartPos(Point pos)
{
	deleteSprite->setStartPos(pos);
}

void GridManagerLayer::setDeleteSpriteMovedPos(Point pos)
{
	deleteSprite->setMovedPos(pos);
}

void GridManagerLayer::setDeleteSpriteEndedPos(Point pos)
{
	deleteSprite->setEndedPos(pos);
	std::vector<Point> poss = deleteSprite->getRange(gridWidth, gridHeight);
	int size = poss.size();
	if (!gridSpriteLayer || BBSize <= 0)
		return;
	for (int i = 0; i < size; i++){
		int x = poss.at(i).x;
		int y = poss.at(i).y;
		if (x >= this->col || y >= this->row || x < 0 || y < 0)
			continue;
		if (BBvec && BBvec[y][x] != NULL){
			auto block = BBvec[y][x];
			block->Remove();
			BBvec[y][x] = NULL;
			BBSize--;
		}
	}
	this->gridSpriteLayer->setDirty(true);
}

void GridManagerLayer::setTouchSpriteMovedPos(Point pos)
{
	touchSprite->setMovedPos(pos);
}

void GridManagerLayer::setTouchSpriteEndedPos(Point pos)
{
	touchSprite->setEndedPos(pos);
	if (touchSprite->getType()){
		Point pos = this->setItalicPosAndTouchPos(0);
		this->gridSpriteLayer->addBoxAndItalic(ItalicPos, pos, BBSize);
	}
}

void GridManagerLayer::setSelectDirectionType(int type)
{
	selectDirectionSprite->setType(type);
}

void GridManagerLayer::setMultiSelectBlockStartPos(Point pos)
{
	multiselectBlock->setStartPos(pos);
}

void GridManagerLayer::setMultiSelectBlockMovedPos(Point pos)
{
	multiselectBlock->setMovedPos(pos);
}

void GridManagerLayer::setMultiSelectBlockEndedPos(Point pos)
{
	multiselectBlock->setBBvec(BBvec, row, col, BBSize, gridWidth, gridHeight);
	multiselectBlock->setEndedPos(pos);
}

void GridManagerLayer::setGridSpriteLayerStartPos(Point pos)
{
	if (gridSpriteLayer)gridSpriteLayer->setStartPos(pos);
}

void GridManagerLayer::setGridSpriteLayerMovedPos(Point pos)
{
	
}

void GridManagerLayer::setGridSpriteLayerEndedPos(Point pos)
{
	if (gridSpriteLayer)gridSpriteLayer->setEndedPos(pos);
}

void GridManagerLayer::setStartPos(Point pos)
{
	startPos = pos;
	ItalicPos = pos;
	setGridSpriteLayerStartPos(pos);
}

void GridManagerLayer::setMovedPos(Point pos)
{
	movedPos = pos;
}

void GridManagerLayer::setEndedPos(Point pos)
{
	endedPos = pos;
	setGridSpriteLayerEndedPos(pos);
}

void GridManagerLayer::createGrid(int row, int col, int height, int width, bool byWinSize)
{
	if (drawnode){
		drawnode->removeFromParentAndCleanup(true);
		drawnode = NULL;
	}
	drawnode = DrawNode::create();
	addChild(drawnode, GridZRoder_bg);
	if (BBvec){
		for (int i = 0; i < this->row; i++){
			for (int j = 0; j < this->col; j++){
				if (BBvec[i][j] != NULL){
					auto block = BBvec[i][j];
					block->Remove();
					BBvec[i][j] = NULL;
				}
			}
			delete BBvec[i];
		}
		delete BBvec;
		BBvec = NULL;
	}
	BBvec = new BaseBlock**[row];
	for (int i = 0; i < row; i++){
		BBvec[i] = new BaseBlock*[col];
		for (int j = 0; j < col; j++){
			BBvec[i][j] = NULL;
		}
	}
	BBSize = 0;
	gridWidth = width;
	gridHeight = height;
	this->row = row;
	this->col = col;
	if (gridSpriteLayer){
		gridSpriteLayer->removeFromParent();
		gridSpriteLayer->release();
		gridSpriteLayer = NULL;
	}
	gridSpriteLayer = GridSpriteLayer::create(row, col, gridWidth, gridHeight);
	gridSpriteLayer->setBaseBlock(BBvec);
	addChild(gridSpriteLayer, GridZRoder_grid);
	char str[256];
	sprintf(str, "创建的网格信息:row:%d col:%d, width:%d height:%d", this->row, this->col, gridWidth, gridHeight);
	addConsoleInfo(str);
	auto glview = Director::getInstance()->getOpenGLView();
	auto size = glview->getFrameSize();
	if (byWinSize == false){
		size = Size(g_CocosWindowInitSize.width(), g_CocosWindowInitSize.height());
	}
	for (int i = 0; i <= row; i++){
		drawnode->drawLine(Vec2(0, size.height - i*height), Vec2(width*col, size.height - i*height), Color4F(gridColor));
	}
	for (int i = 0; i <= col; i++){
		drawnode->drawLine(Vec2(i*width, size.height - row*height), Vec2(i*width, size.height), Color4F(gridColor));
	}
	multiselectBlock->clearDrawNode();
}

bool GridManagerLayer::addGrid(int count, int direction)
{
	auto glview = Director::getInstance()->getOpenGLView();
	if (drawnode){
		drawnode->removeFromParentAndCleanup(true);
		drawnode = NULL;
	}
	drawnode = DrawNode::create();
	addChild(drawnode, GridZRoder_bg);
	for (int i = 0; i <= row; i++){
		drawnode->drawLine(Vec2(0, glview->getFrameSize().height - i*gridHeight), Vec2(gridWidth*col, glview->getFrameSize().height - i*gridHeight), Color4F(gridColor));
	}
	for (int i = 0; i <= col; i++){
		drawnode->drawLine(Vec2(i*gridWidth, glview->getFrameSize().height - row*gridHeight), Vec2(i*gridWidth, glview->getFrameSize().height), Color4F(gridColor));
	}
	BaseBlock*** tmpvec = new BaseBlock**[row];
	for (int i = 0; i < row; i++){
		tmpvec[i] = new BaseBlock*[col];
		for (int j = 0; j < col; j++){
			tmpvec[i][j] = NULL;
		}
	}
	int oldrow = 0;
	switch (direction){
		//上
	case 0:
		for (int i = 0; i < row - count; i++){
			for (int j = 0; j < col; j++){
				if (BBvec[i][j] != NULL){
					tmpvec[i + count][j] = BBvec[i][j];
					tmpvec[i + count][j]->setTag(tmpvec[i + count][j]->getTag() + count*col);
					auto size = tmpvec[i + count][j]->getContentSize();
					float y = glview->getFrameSize().height - ((i + count) * size.height + size.height / 2);
					float x = j * size.width + size.width / 2;
					tmpvec[i + count][j]->setPosition(x, y);
				}
			}
		}
		oldrow = row - count;
		break;
		//下
	case 1:
		for (int i = 0; i < row - count; i++){
			for (int j = 0; j < col; j++){
				if (BBvec[i][j] != NULL){
					tmpvec[i][j] = BBvec[i][j];
					tmpvec[i][j]->setTag(BBvec[i][j]->getTag());
					auto size = tmpvec[i][j]->getContentSize();
					float y = glview->getFrameSize().height - (i * size.height + size.height / 2);
					float x = j * size.width + size.width / 2;
					tmpvec[i][j]->setPosition(x, y);
				}
			}
		}
		oldrow = row - count;
		break;
		//左
	case 2:
		for (int i = 0; i < row; i++){
			for (int j = 0; j < col - count; j++){
				if (BBvec[i][j] != NULL){
					tmpvec[i][j + count] = BBvec[i][j];
					tmpvec[i][j + count]->setTag(i* col + j + count);
					auto size = tmpvec[i][j + count]->getContentSize();
					float y = glview->getFrameSize().height - (i * size.height + size.height / 2);
					float x = (j + count) * size.width + size.width / 2;
					tmpvec[i][j + count]->setPosition(x, y);
				}
			}
		}
		oldrow = row;
		break;
		//右
	case 3:
		for (int i = 0; i < row; i++){
			for (int j = 0; j < col - count; j++){
				if (BBvec[i][j] != NULL){
					tmpvec[i][j] = BBvec[i][j];
					tmpvec[i][j]->setTag(BBvec[i][j]->getTag() + count*i);
					auto size = tmpvec[i][j]->getContentSize();
					float y = glview->getFrameSize().height - (i * size.height + size.height / 2);
					float x = j * size.width + size.width / 2;
					tmpvec[i][j]->setPosition(x, y);
				}
			}
		}
		oldrow = row;
		break;
	}
	if (BBvec){
		for (int i = 0; i < oldrow; i++){
			delete BBvec[i];
		}
		delete BBvec;
		BBvec = NULL;
	}
	BBvec = tmpvec;
	gridSpriteLayer->setGrid(this->row, this->col);
	gridSpriteLayer->setBaseBlock(BBvec);
	gridSpriteLayer->updateGrid();
	multiselectBlock->clearDrawNode();
	return true;
}

void GridManagerLayer::setDrawColor(Color4B b)
{
	gridColor = b;
	auto glview = Director::getInstance()->getOpenGLView();
	if (drawnode){
		drawnode->removeFromParentAndCleanup(true);
		drawnode = NULL;
	}
	drawnode = DrawNode::create();
	addChild(drawnode, GridZRoder_bg);
	for (int i = 0; i <= row; i++){
		drawnode->drawLine(Vec2(0, glview->getFrameSize().height - i*gridHeight), Vec2(gridWidth*col, glview->getFrameSize().height - i*gridHeight), Color4F(gridColor));
	}
	for (int i = 0; i <= col; i++){
		drawnode->drawLine(Vec2(i*gridWidth, glview->getFrameSize().height - row*gridHeight), Vec2(i*gridWidth, glview->getFrameSize().height), Color4F(gridColor));
	}
}

Color4B GridManagerLayer::getDrawColor()
{
	return gridColor;
}

int GridManagerLayer::getTouchSpriteType()
{
	return touchSprite->getType();
}

GridSpriteLayer* GridManagerLayer::getGridSpriteLayer()
{
	return gridSpriteLayer;
}

DrawNode* GridManagerLayer::getDrawNode()
{
	return drawnode;
}

void GridManagerLayer::addBlock(BaseBlock* block, int row, int col)
{
	gridSpriteLayer->addChild(block);
	BBvec[row][col] = block;
	BBSize++;
}

BaseBlock* GridManagerLayer::getBlock(int row, int col)
{
	return BBvec[row][col];
}

int GridManagerLayer::getBBSize()
{
	return BBSize;
}

BaseBlock*** GridManagerLayer::getBBvec()
{
	return BBvec;
}

void GridManagerLayer::clearDraw()
{
	multiselectBlock->clearDrawNode();
}

void GridManagerLayer::pressUp()
{
	multiselectBlock->up();
}

void GridManagerLayer::pressDown()
{
	multiselectBlock->down();
}

void GridManagerLayer::pressRight()
{
	multiselectBlock->right();
}

void GridManagerLayer::pressLeft()
{
	multiselectBlock->left();
}

void GridManagerLayer::releaseUp()
{
	
}

void GridManagerLayer::releaseDown()
{
	
}

void GridManagerLayer::releaseLeft()
{
	
}

void GridManagerLayer::releaseRight()
{
	
}