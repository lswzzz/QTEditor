#include "MultiselectBlock.h"
#include "Global.h"
#include "GridSpriteLayer.h"

MultiselectBlock::MultiselectBlock()
{
	drawnode = NULL;
	BBvec = NULL;
	selectVec = NULL;
	row = 0;
	col = 0;
}

MultiselectBlock::~MultiselectBlock()
{
	if (drawnode){
		drawnode->removeFromParent();
		drawnode = NULL;
	}
	deleteSelectVec();
}

MultiselectBlock* MultiselectBlock::create()
{
	auto ret = new MultiselectBlock;
	if (ret && ret->init()){
		return ret;
	}
	return nullptr;
}

void MultiselectBlock::deleteSelectVec()
{
	if (selectVec){
		for (int i = 0; i < row; i++){
			delete[] selectVec[i];
		}
		delete[] selectVec;
	}
	selectVec = NULL;
}

void MultiselectBlock::createSelectVec()
{
	selectVec = new bool*[row];
	for (int i = 0; i < row; i++){
		selectVec[i] = new bool[col];
		for (int j = 0; j < col; j++){
			selectVec[i][j] = false;
		}
	}
}

bool MultiselectBlock::init()
{
	if (!Sprite::init()){
		return false;
	}
	drawRect = false;
	cur_static = 0;
	return true;
}

void MultiselectBlock::setStartPos(Point pos)
{
	startPos = pos;
	drawRect = false;
	cur_static = 1;
}

void MultiselectBlock::setMovedPos(Point pos)
{
	movedPos = pos;
	drawRect = true;
	if (cur_static == 1)
		cur_static = 2;
}

void MultiselectBlock::setEndedPos(Point pos)
{
	endedPos = pos;
	drawRect = false;
	cur_static = 0;
	std::vector<Point> poss = getRange(gridW, gridH);
	int size = poss.size();
	for (int i = 0; i < size; i++){
		int x = poss.at(i).x;
		int y = poss.at(i).y;
		if (x >= col || y >= row || x < 0 || y < 0)
			continue;
		if (BBvec && BBvec[y][x] != NULL){
			addToVec(Point(x, y));
		}
	}
	addDrawNode();
}

void MultiselectBlock::addToVec(Point loc)
{
	int ox = loc.x;
	int oy = loc.y;
	if (selectVec[oy][ox] == false)
		selectVec[oy][ox] = true;
	else{
		selectVec[oy][ox] = false;
	}
}

void MultiselectBlock::addDrawNode()
{
	if (!drawnode){
		drawnode = DrawNode::create();
		addChild(drawnode);
	}
	drawnode->clear();
	Color4F color = Color4F(Color4B(0xCD, 0x00, 0x00, 0xFF));
	auto frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	for (int i = 0; i < row; i++){
		for (int j = 0; j < col; j++){
			if (selectVec[i][j]){
				int height = 0;
				if (g_CocosWindowInitSize.height() != getNewWindowSize().height()){
					height += getOldWindowSize().height() - getNewWindowSize().height();
				}
				if (i - 1 >= 0){
					if (!selectVec[i - 1][j]){
						drawnode->drawLine(Point(j*gridW, frameSize.height - i * gridH + height), Point((j + 1)*gridW, frameSize.height - i * gridH + height), color);
					}
				}
				else{
					drawnode->drawLine(Point(j*gridW, frameSize.height - i * gridH + height), Point((j + 1)*gridW, frameSize.height - i * gridH + height), color);
				}
				if (j - 1 >= 0){
					if (!selectVec[i][j - 1]){
						drawnode->drawLine(Point(j *gridW, frameSize.height - i * gridH + height), Point(j*gridW, frameSize.height - (i + 1)* gridH + height), color);
					}
				}
				else{
					drawnode->drawLine(Point(j *gridW, frameSize.height - i * gridH + height), Point(j*gridW, frameSize.height - (i + 1)* gridH + height), color);
				}
				if (j + 1 < col){
					if (!selectVec[i][j + 1]){
						drawnode->drawLine(Point((j + 1)* gridW, frameSize.height - i * gridH + height), Point((j + 1)*gridW, frameSize.height - (i + 1)* gridH + height), color);
					}
				}
				else{
					drawnode->drawLine(Point((j + 1)* gridW, frameSize.height - i * gridH + height), Point((j + 1)*gridW, frameSize.height - (i + 1)* gridH + height), color);
				}
				if (i + 1 < row){
					if (!selectVec[i + 1][j]){
						drawnode->drawLine(Point(j*gridW, frameSize.height - (i + 1)* gridH + height), Point((j + 1)*gridW, frameSize.height - (i + 1)* gridH + height), color);
					}
				}
				else{
					drawnode->drawLine(Point(j*gridW, frameSize.height - (i + 1)* gridH + height), Point((j + 1)*gridW, frameSize.height - (i + 1)* gridH + height), color);
				}
					
			}
		}
	}
}

void MultiselectBlock::clearDraw()
{
	for (int i = 0; i < row; i++){
		for (int j = 0; j < col; j++){
			if (selectVec[i][j]){
				auto block = BBvec[i][j];
				block->clearDraw();
			}
		}
	}
}

void MultiselectBlock::resetVec(int direct)
{
	switch (direct){
		//up
	case 1:
		for (int i = 0; i < row; i++){
			for (int j = 0; j < col; j++){
				if (selectVec[i][j]){
					auto block = BBvec[i][j];
					BBvec[i][j] = NULL;
					BBvec[i - 1][j] = block;
					selectVec[i][j] = false;
					selectVec[i - 1][j] = true;
				}
			}
		}
		break;
		//down
	case 2:
		for (int i = row-1; i >= 0; i--){
			for (int j = 0; j < col; j++){
				if (selectVec[i][j]){
					auto block = BBvec[i][j];
					BBvec[i][j] = NULL;
					BBvec[i + 1][j] = block;
					selectVec[i][j] = false;
					selectVec[i + 1][j] = true;
				}
			}
		}
		break;
		//left
	case 3:
		for (int i = 0; i < row; i++){
			for (int j = 0; j < col; j++){
				if (selectVec[i][j]){
					auto block = BBvec[i][j];
					BBvec[i][j] = NULL;
					BBvec[i][j-1] = block;
					selectVec[i][j] = false;
					selectVec[i][j-1] = true;
				}
			}
		}
		break;
		//right
	case 4:
		for (int i = 0; i < row; i++){
			for (int j = col-1; j >= 0; j--){
				if (selectVec[i][j]){
					auto block = BBvec[i][j];
					BBvec[i][j] = NULL;
					BBvec[i][j+1] = block;
					selectVec[i][j] = false;
					selectVec[i][j+1] = true;
				}
			}
		}
		break;
	}
}

void MultiselectBlock::resetDrawUp()
{
	for (int i = 0; i < row; i++){
		for (int j = 0; j < col; j++){
			if (selectVec[i][j]){
				switch (BBvec[i][j]->getType()){
				case Type_Box:
				{
					auto block = dynamic_cast<BoxBody*>(BBvec[i][j]);
					block->setPositionY(block->getPositionY() + gridH);
					int tag = block->getTag();
					int y = tag / col;
					int x = tag % col;
					y--;
					block->setTag(y*col + x);
					block->DrawGrid();
				}
					break;
				case Type_Italic:
				{
					auto block = dynamic_cast<ItalicBody*>(BBvec[i][j]);
					block->setPositionY(block->getPositionY() + gridH);
					int tag = block->getTag();
					int y = tag / col;
					int x = tag % col;
					y--;
					block->setTag(y*col + x);
					block->addDrawNode(block->getPos1(), block->getPos2(), block->getDrawType());
				}
					break;
				}

			}
		}
	}
}

void MultiselectBlock::resetDrawDown()
{
	for (int i = row-1; i >= 0; i--){
		for (int j = 0; j < col; j++){
			if (selectVec[i][j]){
				switch (BBvec[i][j]->getType()){
				case Type_Box:
				{
					auto block = dynamic_cast<BoxBody*>(BBvec[i][j]);
					block->setPositionY(block->getPositionY() - gridH);
					int tag = block->getTag();
					int y = tag / col;
					int x = tag % col;
					y++;
					block->setTag(y*col + x);
					block->DrawGrid();
				}
					break;
				case Type_Italic:
				{
					auto block = dynamic_cast<ItalicBody*>(BBvec[i][j]);
					block->setPositionY(block->getPositionY() - gridH);
					int tag = block->getTag();
					int y = tag / col;
					int x = tag % col;
					y++;
					block->setTag(y*col + x);
					block->addDrawNode(block->getPos1(), block->getPos2(), block->getDrawType());
				}
					break;
				}

			}
		}
	}
}

void MultiselectBlock::resetDrawLeft()
{
	for (int i = 0; i < row; i++){
		for (int j = 0; j < col; j++){
			if (selectVec[i][j]){
				switch (BBvec[i][j]->getType()){
				case Type_Box:
				{
					auto block = dynamic_cast<BoxBody*>(BBvec[i][j]);
					block->setPositionX(block->getPositionX() - gridW);
					int tag = block->getTag();
					int y = tag / col;
					int x = tag % col;
					x--;
					block->setTag(y*col + x);
					block->DrawGrid();
				}
					break;
				case Type_Italic:
				{
					auto block = dynamic_cast<ItalicBody*>(BBvec[i][j]);
					block->setPositionX(block->getPositionX() - gridW);
					int tag = block->getTag();
					int y = tag / col;
					int x = tag % col;
					x--;
					block->setTag(y*col + x);
					block->addDrawNode(block->getPos1(), block->getPos2(), block->getDrawType());
				}
					break;
				}

			}
		}
	}
}

void MultiselectBlock::resetDrawRight()
{
	for (int i = 0; i < row; i++){
		for (int j = col-1; j >= 0; j--){
			if (selectVec[i][j]){
				switch (BBvec[i][j]->getType()){
				case Type_Box:
				{
					auto block = dynamic_cast<BoxBody*>(BBvec[i][j]);
					block->setPositionX(block->getPositionX() + gridW);
					int tag = block->getTag();
					int y = tag / col;
					int x = tag % col;
					x++;
					block->setTag(y*col + x);
					block->DrawGrid();
				}
					break;
				case Type_Italic:
				{
					auto block = dynamic_cast<ItalicBody*>(BBvec[i][j]);
					block->setPositionX(block->getPositionX() + gridW);
					int tag = block->getTag();
					int y = tag / col;
					int x = tag % col;
					x++;
					block->setTag(y*col + x);
					block->addDrawNode(block->getPos1(), block->getPos2(), block->getDrawType());
				}
					break;
				}

			}
		}
	}
}

bool MultiselectBlock::checkMoveUp()
{
	for (int j = 0; j < col; j++){
		for (int i = 0; i < row; i++){
			if (selectVec[i][j]){
				if (i == 0){
					return false;
				}
				if (BBvec[i - 1][j] != NULL){
					return false;
				}
				break;
			}
		}
	}
	return true;
}

bool MultiselectBlock::checkMoveDown()
{
	for (int j = 0; j < col; j++){
		for (int i = row - 1; i >= 0; i--){
			if (selectVec[i][j]){
				if (i == row - 1){
					return false;
				}
				if (BBvec[i + 1][j] != NULL){
					return false;
				}
				break;
			}
		}
	}
	return true;
}

bool MultiselectBlock::checkMoveLeft()
{
	for (int i = 0; i < row; i++){
		for (int j = 0; j < col; j++){
			if (selectVec[i][j]){
				if (j == 0){
					return false;
				}
				if (BBvec[i][j-1] != NULL){
					return false;
				}
				break;
			}
		}
	}
	return true;
}

bool MultiselectBlock::checkMoveRight()
{
	for (int i = 0; i < row; i++){
		for (int j = col - 1; j >= 0; j--){
			if (selectVec[i][j]){
				if (j == col - 1){
					return false;
				}
				if (BBvec[i][j + 1] != NULL){
					return false;
				}
				break;
			}
		}
	}
	return true;
}

void MultiselectBlock::up()
{
	if (!checkMoveUp()){
		addConsoleInfo("无法向上移动");
		return;
	}
	clearDraw();
	resetDrawUp();
	resetVec(1);
	addDrawNode();
}

void MultiselectBlock::down()
{
	if (!checkMoveDown()){
		addConsoleInfo("无法向下移动");
		return;
	}
	clearDraw();
	resetDrawDown();
	resetVec(2);
	addDrawNode();
}

void MultiselectBlock::left()
{
	if (!checkMoveLeft()){
		addConsoleInfo("无法向左移动");
		return;
	}
	clearDraw();
	resetDrawLeft();
	resetVec(3);
	addDrawNode();
}

void MultiselectBlock::right()
{
	if (!checkMoveRight()){
		addConsoleInfo("无法向右移动");
		return;
	}
	clearDraw();
	resetDrawRight();
	resetVec(4);
	addDrawNode();
}

void MultiselectBlock::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(MultiselectBlock::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

void MultiselectBlock::onDraw(const Mat4 &transform, uint32_t flags)
{
	if (drawRect && cur_static > 0){
		Director* director = Director::getInstance();
		director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
		director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
		CHECK_GL_ERROR_DEBUG();
		glLineWidth(2.0f);
		DrawPrimitives::setDrawColor4B(0xDE, 0xB8, 0x87, 255);
		Point pos1, pos2, pos3, pos4;
		pos1 = startPos;
		pos2 = Point(startPos.x, movedPos.y);
		pos3 = movedPos;
		pos4 = Point(movedPos.x, startPos.y);
		DrawPrimitives::drawLine(pos1, pos2);
		DrawPrimitives::drawLine(pos2, pos3);
		DrawPrimitives::drawLine(pos3, pos4);
		DrawPrimitives::drawLine(pos4, pos1);
		CHECK_GL_ERROR_DEBUG();
		director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	}
}

void MultiselectBlock::setBBvec(BaseBlock*** vec, int row, int col, int size, int gridW, int gridH)
{
	if (BBvec != vec){
		deleteSelectVec();
		BBvec = vec;
		this->row = row;
		this->col = col;
		createSelectVec();
	}
	this->gridW = gridW;
	this->gridH = gridH;
}

void MultiselectBlock::clearDrawNode()
{
	if (drawnode){
		drawnode->clear();
	}
	for (int i = 0; i < row; i++){
		for (int j = 0; j < col; j++){
			selectVec[i][j] = false;
		}
	}
}

std::vector<Point> MultiselectBlock::getRange(int gridW, int gridH)
{
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	float width1 = startPos.x;
	float height1 = glview->getFrameSize().height - startPos.y;
	float width2 = endedPos.x;
	float height2 = glview->getFrameSize().height - endedPos.y;
	if (g_CocosWindowInitSize.height() != getNewWindowSize().height()){
		height1 += getOldWindowSize().height() - getNewWindowSize().height();
		height2 += getOldWindowSize().height() - getNewWindowSize().height();
	}
	int pos1x = width1 / gridW;
	int pos1y = height1 / gridH;
	int pos2x = width2 / gridW;
	int pos2y = height2 / gridH;
	int temp;
	if (pos1x > pos2x){
		temp = pos1x;
		pos1x = pos2x;
		pos2x = temp;
	}
	if (pos1y > pos2y){
		temp = pos1y;
		pos1y = pos2y;
		pos2y = temp;
	}
	std::vector<Point> poss;
	for (int i = pos1y; i <= pos2y; i++){
		for (int j = pos1x; j <= pos2x; j++){
			poss.push_back(Point(j, i));
		}
	}
	return poss;
}