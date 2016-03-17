#include "FluidBody.h"


FluidBody::FluidBody()
{
}


FluidBody::~FluidBody()
{
}

FluidBody* FluidBody::create(std::string file, float width, float height)
{
	auto sprite = new FluidBody();
	if (sprite){
		sprite->initData(width, height);
		return sprite;
	}
	else{
		return NULL;
	}
}