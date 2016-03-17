#include "LoadExterModel.h"
#include "Role3D.h"

LoadExterModel::LoadExterModel()
{
}


LoadExterModel::~LoadExterModel()
{
}

void LoadExterModel::loadRole(Node* node, std::string path, std::string pngpath)
{
	Role3D* role;
	if (pngpath == ""){
		role = Role3D::createWithC3T(path);
	}
	else{
		role = Role3D::createWithC3T(path, pngpath);
	}
	node->addChild(role, 100);
	auto frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	role->setPosition(Point(frameSize.width / 2, frameSize.height / 2));
	role->setRotation3D(Vec3(0, -90, 0));
}

void LoadExterModel::loadPng(Node* node, std::string path)
{

}