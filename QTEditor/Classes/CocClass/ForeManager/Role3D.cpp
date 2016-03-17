#include "Role3D.h"


Role3D::Role3D()
{
}


Role3D::~Role3D()
{
}

Role3D* Role3D::createWithC3T(const std::string& path)
{
	if (path.length() < 0){
		CCASSERT(false, "invalid filename for Sprite3D");
	}
	Role3D* ret = new(std::nothrow) Role3D();
	if (ret && ret->initWithFile(path)){
		ret->_contentSize = ret->getBoundingBox().size;
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

Role3D* Role3D::createWithC3T(const std::string& path, const std::string& texturePath)
{
	auto sprite = createWithC3T(path);
	sprite->m_texturePath = texturePath;
	sprite->m_path = path;
	if (sprite){
		sprite->setTexture(texturePath);
	}
	return sprite;
}

bool Role3D::initWithPng(std::string path)
{
	return this->initWithFile(path);
}