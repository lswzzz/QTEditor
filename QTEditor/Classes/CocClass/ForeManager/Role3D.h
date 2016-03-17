#pragma once
#include "cocos2d.h"
#include "ImageSprite.h"

using namespace cocos2d;

class Role3D : public Sprite3D
{
public:
	static Role3D* createWithC3T(const std::string& path);
	static Role3D* createWithC3T(const std::string& path, const std::string& texturePath);
	bool initWithPng(std::string path);
	void setC3DTexture(std::string texturePath);

protected:
	Role3D();
	virtual ~Role3D();
	std::string m_path;
	std::string m_texturePath;
};

