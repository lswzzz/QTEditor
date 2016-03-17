#include "CancelSaveData.h"
#include "Global.h"
#include "ImageSprite.h"
#include "HelloWorldScene.h"
#include "LayerManagerWidget.h"
#include "TemplateView.h"

CancelSaveData::CancelSaveData()
{
	maxSize = 10;
}

CancelSaveData* CancelSaveData::create()
{
	auto data = new CancelSaveData();
	return data;
}

void CancelSaveData::setMaxSize(int size)
{
	maxSize = size;
}

CancelSaveData::~CancelSaveData()
{
	for (int i = 0; i < save_V.size(); i++){
		auto data = save_V.at(i);
		delete data;
	}
	save_V.clear();
}

void CancelSaveData::addDeleteData(Vector<Node*> vec)
{
	vector<Node*> vec_;
	for (int i = 0; i < vec.size(); i++){
		vec_.push_back(vec.at(i));
	}
	addDeleteData(vec_);
}

void CancelSaveData::addDeleteData(vector<Node*> vec)
{
	saveData* data = new saveData;
	data->vec = vec;
	data->_type = __DELETE;
	for (int i = 0; i < data->vec.size(); i++){
		auto pos = data->vec.at(i)->getPosition();
		auto sprite = data->vec.at(i);
		sprite->retain();
		sprite->setVisible(false);
		data->posV.push_back(pos);
	}
	addSaveData(data);
}

void CancelSaveData::addDeleteData(Node* node)
{
	saveData* data = new saveData;
	data->vec.push_back(node);
	data->_type = __DELETE;
	node->retain();
	node->setVisible(false);
	data->posV.push_back(node->getPosition());
	addSaveData(data);
}

void CancelSaveData::addSaveData(saveData* data)
{
	if (save_V.size() < maxSize){
		save_V.push_back(data);
	}
	else{
		auto da = save_V.at(0);
		vector<saveData*>::iterator iter = save_V.begin();
		save_V.erase(iter);
		if (da->_type == __DELETE){
			for (int i = 0; i < da->vec.size(); i++){
				auto sprite = da->vec.at(i);
				sprite->release();
			}
		}
		delete da;
		save_V.push_back(data);
	}
}

CancelSaveData::saveData* CancelSaveData::getSaveData()
{
	if (save_V.size() > 0){
		auto data = save_V.at(save_V.size() - 1);
		save_V.pop_back();
		return data;
	}
	else{
		return NULL;
	}
}

void CancelSaveData::addSaveData(vector<Node*> vec)
{
	saveData* data = new saveData;
	data->vec = vec;
	data->_type = __Move;
	for (int i = 0; i < data->vec.size(); i++){
		auto pos = data->vec.at(i)->getPosition();
		data->posV.push_back(pos);
	}
	addSaveData(data);
}

void CancelSaveData::addSaveData(Vector<Node*> vec)
{
	vector<Node*> vec_;
	for (int i = 0; i < vec.size(); i++){
		vec_.push_back(vec.at(i));
	}
	addSaveData(vec_);
}

void CancelSaveData::addSaveData(Node* node)
{
	saveData* data = new saveData;
	data->vec.push_back(node);
	data->_type = __Move;
	data->posV.push_back(node->getPosition());
	addSaveData(data);
}

void CancelSaveData::pressAltZ()
{
	auto data = getSaveData();
	if (data){
		if (data->_type == __Move){
			for (int i = 0; i < data->vec.size(); i++){
				auto sprite = dynamic_cast<ImageSprite*>(data->vec.at(i));
				auto pos = data->posV.at(i);
				sprite->setPosition(pos);
			}
			delete data;
		}
		else{
			auto scene = dynamic_cast<HelloWorld*>(g_scene);
			for (int i = 0; i < data->vec.size(); i++){
				auto sprite = dynamic_cast<ImageSprite*>(data->vec.at(i));
				sprite->setVisible(true);
				auto parentTag = sprite->getParentTag();
				auto layer = scene->getLayerFromForeManager(parentTag);
				layer->addChild(sprite, sprite->getZOrder());
				auto layerManager = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
				layerManager->addItemSpriteAsyn(parentTag.c_str(), sprite->getTagName().c_str(), 0, sprite);
				auto templateView = dynamic_cast<TemplateView*>(g_TemplateView);
				templateView->itemAddSprite(sprite);
				sprite->release();
			}
			delete data;
		}
	}
}