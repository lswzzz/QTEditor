#include "AdsorbentLayer.h"
#include "Global.h"

AdsorbentLayer::AdsorbentLayer()
{
	row = 0;
	col = 0;
	rowheight = 0;
	colwidth = 0;
	isDraw = false;
}

AdsorbentLayer::~AdsorbentLayer()
{
}

AdsorbentLayer* AdsorbentLayer::create()
{
	auto ret = new AdsorbentLayer;
	if (ret && ret->init()){
		return ret;
	}
	return nullptr;
}

void AdsorbentLayer::setGrid(int row, int col, int rowheight, int colwidth)
{
	this->row = row;
	this->col = col;
	this->rowheight = rowheight;
	this->colwidth = colwidth;
}

void AdsorbentLayer::setShowLine(Point pos, Size size)
{
	auto glview = Director::getInstance()->getOpenGLView();
	int minX, maxX, minY, maxY;
	minX = pos.x - size.width / 2;
	maxY = glview->getFrameSize().height - (pos.y - size.height / 2);
	maxX = pos.x + size.width / 2;
	minY = glview->getFrameSize().height - (pos.y + size.height / 2);

	int col1 = minX / colwidth;
	int row1 = minY / rowheight;
	int col2 = maxX / colwidth;
	int row2 = maxY / rowheight;
	//点1
	float width1 = col1* colwidth;
	float height1 = row1 * rowheight;
	//点2
	float width2 = (col2 + 1) * colwidth;
	float height2 = (row2 + 1) * rowheight;
	if (g_CocosWindowInitSize.height() != getNewWindowSize().height()){
		int off = getOldWindowSize().height() - getNewWindowSize().height();
		if (off < 0){
			off = rowheight - (-off) % rowheight;
			off = -off;
		}
		else{
			off = rowheight - off%rowheight;
		}
		height1 += off;
		height2 += off;
	}
	int x = 0, y = 0;
	int typex = 0;
	int typey = 0;
	//width1和height1可能小于0要进行检测
	if (minX - width1 <= colwidth / 2 && width1 >= 0 && size.width >= colwidth){
		x = width1;
		typex = 1;
	}
	else if (width2 - maxX <= colwidth / 2 && size.width >= colwidth){
		x = width2;
		typex = 2;
	}
	else if (minX - width1 <= 10 && width1 >= 0 && size.width < colwidth){
		x = width1;
		typex = 1;
	}
	else if (width2 - maxX <= 10 && size.width < colwidth){
		x = width2;
		typex = 2;
	}
	if (minY - height1 <= rowheight / 2 && height1 >= 0 && size.height >= rowheight){
		y = height1;
		typey = 1;
	}
	else if (maxY - height2 <= rowheight / 2 && size.height >= rowheight){
		y = height2;
		typey = 2;
	}
	else if (minY - height1 <= 10 && height1 >= 0 && size.height < rowheight){
		y = height1;
		typey = 1;
	}
	else if (maxY - height2 <= 10 && size.height < rowheight){
		y = height2;
		typey = 2;
	}
	if (typex != 0 && typey != 0){
		isDraw = true;
		y = glview->getFrameSize().height - y;
		pos1 = Point(x, y);
		pos3 = Point(x, y);
		int width = maxX - minX;
		int height = maxY - minY;
		//左上角
		if (typex == 1 && typey == 1){
			pos2 = Point(x, y - height);
			pos4 = Point(x + width, y);
			
			targetPos = Point(x + width / 2, y - height / 2);
		
		//右上角
		}else if(typex == 2 && typey == 1){
			pos2 = Point(x, y - height);
			pos4 = Point(x - width, y);
			
			targetPos = Point(x - width / 2, y - height / 2);
		
		}
		//左下角
		else if (typex == 1 && typey == 2){
			pos2 = Point(x, y +  height);
			pos4 = Point(x + width, y);
			
			targetPos = Point(x + width / 2, y + height / 2);
			
		}
		//右下角
		else{
			pos2 = Point(x, y + height);
			pos4 = Point(x - width, y);
			
			targetPos = Point(x - width / 2, y + height / 2);
			
		}
	}
	else{
		isDraw = false;
	}
}

void AdsorbentLayer::setShowLine(cocos2d::Point pos, Node* node)
{
	auto size = Size(node->getContentSize().width*node->getScaleX(),
		node->getContentSize().height*node->getScaleY());
	trans = Size(size.width*(0.5 - node->getAnchorPoint().x), size.height*(0.5 - node->getAnchorPoint().y));
	pos = pos + trans;
	setShowLine(pos, size);
}

void AdsorbentLayer::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(AdsorbentLayer::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

void AdsorbentLayer::onDraw(const Mat4 &transform, uint32_t flags)
{
	if (!isDraw)
		return;
	Director* director = Director::getInstance();
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
	CHECK_GL_ERROR_DEBUG();
	glLineWidth(2.0f);
	DrawPrimitives::setDrawColor4B(0, 255, 0, 255);
	DrawPrimitives::drawLine(pos1, pos2);
	DrawPrimitives::drawLine(pos3, pos4);
	CHECK_GL_ERROR_DEBUG();
	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void AdsorbentLayer::setIsDraw(bool isdraw)
{
	isDraw = isdraw;
}

bool AdsorbentLayer::getIsDraw()
{
	return isDraw;
}

Point AdsorbentLayer::getTargetPos()
{
	return targetPos - trans;
}