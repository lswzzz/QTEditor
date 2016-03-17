#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class LoadExterModel
{
public:
	LoadExterModel();
	void loadRole(Node* node, std::string path, std::string pngpath = "");
	void loadPng(Node* node, std::string path);
	virtual ~LoadExterModel();
};

