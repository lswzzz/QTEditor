#pragma once
#include "BaseBlock.h"



class FluidBody : virtual public BaseBlock
{
public:
	
	static FluidBody* create(std::string file, float width, float height);
protected:
	FluidBody();
	virtual ~FluidBody();
};

