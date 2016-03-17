#include "ItalicBody.h"
#include "Global.h"
#include "GridSpriteLayer.h"

ItalicBody::ItalicBody()
{
	drawnode = NULL;
	need = false;
}


ItalicBody::~ItalicBody()
{
	if (drawnode){
		drawnode->removeFromParent();
	}
}

ItalicBody* ItalicBody::create(std::string file, float width, float height)
{
	auto sprite = new ItalicBody();
	if (sprite){;
		sprite->initData(width, height);
		return sprite;
	}
	else{
		return NULL;
	}
}

//drawPolygon
//pos1始终是y值比较高的
void ItalicBody::addDrawNode(Point pos1, Point pos2, int type)
{
	this->draw_type = type;
	GridSpriteLayer* layer = dynamic_cast<GridSpriteLayer*>(this->getParent());
	layer->addItalic(this, pos1, pos2, need); 
}

Point ItalicBody::getStartPos()
{
	return start;
}

Point ItalicBody::getEndPos()
{
	return end;
}

void ItalicBody::setStartPos(Point pos)
{
	start = pos;
}

void ItalicBody::setEndPos(Point pos)
{
	end = pos;
}

void ItalicBody::setPos1(Point pos)
{
	pos1 = pos;
}

Point ItalicBody::getPos1()
{
	return pos1;
}

void ItalicBody::setPos2(Point pos)
{
	pos2 = pos;
}

Point ItalicBody::getPos2()
{
	return pos2;
}

int ItalicBody::getDrawType()
{
	return draw_type;
}

bool ItalicBody::isNeed()
{
	return need;
}