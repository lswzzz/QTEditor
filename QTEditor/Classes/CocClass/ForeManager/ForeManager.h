#pragma once
#include "cocos2d.h"
#include "ImageSpriteLayer.h"
#include "ImageSprite.h"
#include "MultiselectSprite.h"
#include "LayerMoveRangeManage.h"

using namespace cocos2d;

class ForeManager : public Layer
{
public:
	
	static ForeManager* create();

	bool addImageLayer(std::string tagname, int zorder);
	void changeSpriteChildTemplate_All(std::string md5, std::string new_templateFile);
	ImageSprite* addSpriteChildFromLayer(cocos2d::Point pos, std::string parent, std::string filename, std::string tag, int zorder, Image_Type type_);
	bool setZOrderFromImageLayerTagName(std::string tagname, int zorder);
	bool setChooseLayer(std::string name);
	bool setChooseSpriteChild(std::string parent, std::string child);
	void setStartPos(cocos2d::Point pos);
	bool MoveLayer(cocos2d::Point pos);
	bool MoveSpriteChildFromLayer(cocos2d::Point pos);
	bool setZOrderFromImageChildTagName(std::string parenttag, std::string tagname, int zorder);
	ImageSpriteLayer* getImageLayerFromTag(std::string tagname);
	ImageSprite* getImageChildFromTag(std::string parenttag,  std::string tagname);
	ImageSprite* getImageChildFromTag(std::string taname);
	ImageSprite* getImageChildFromId(unsigned int id);
	bool removeLayer(std::string name);
	bool removeSpriteChild(std::string parent, std::string name);
	bool changeLayerTagName(std::string oldtag, std::string newtag);
	bool changeSpriteChildTagName(std::string parent, std::string oldtag, std::string newtag);
	bool changeLayerZOrder(std::string name, int zorder);
	bool changeSpriteChildZOrder(std::string parent, std::string name, int zorder);
	std::vector<ImageSpriteLayer*> getALLImageLayer();
	void setLayerVisible(std::string name, bool isVisible);
	void setSpriteChildVisible(std::string parent, std::string name, bool isVisible);
	QVariant getData();
	void setData(QVariant data);
	//0 all 1 static 2 dynamic 如果想获得所有图片使用默认值就可以了
	std::vector<std::string> getImage(int type = 0);
	std::vector<std::string> getAllImage();
	std::vector<std::string> getAllImageNoStatic();
	std::vector<std::vector<std::string>> getALLStaticImageLayerByLayer();
	std::vector<std::string> getAllStaticImageLayerTagName();
	std::vector<std::vector<std::string>> getAllDynamicImageLayerByLayer();
	std::vector<std::string> getAllStaticLayerImage();
	bool hasAnimationSprite();
	std::vector<animateData> getAllAnimateData();
	//使用后要使用下面的deleteStaticMergeLayerVecData方法删除数据
	std::vector<std::vector<ImageSpriteLayer*>*> getAllStaticMergeLayerVec();
	void deleteStaticMergeLayerVecData(std::vector<std::vector<ImageSpriteLayer*>*> vec);
	ImageSprite* getTargetSpriteChild();
	ImageSpriteLayer* getTargetLayer();
	void setSpriteChildPositionFromLayer(cocos2d::Point pos);
	bool ChooseSpriteChildFromPosition(cocos2d::Point pos);

	void setMultiSelectSpriteStartPos(cocos2d::Point pos);
	void setMultiSelectSpriteMovedPos(cocos2d::Point pos);
	void setMultiSelectSpriteEndedPos(cocos2d::Point pos);
	void setMultiSelectSpriteSingleSprite(ImageSprite* sprite);
	bool isTouchMultiSelect(cocos2d::Point pos);

	void clearDraw();
	void moveMultiSelectSprite(cocos2d::Point size);
	void addGridResetPos(int count, int direction, int gridSize);
	void copySpriteFromMultiSelect();
	void PasteSpriteFromMultiSelect();
	Vector<Node*> getMultiSelect();
	void deleteMultiSelect();
	bool isClearDraw(){ return clear; };
	void setLayerLockState(std::string layername, bool lock);

	void pressedUp(bool is_Alt_Key, int size);
	void pressedDown(bool is_Alt_Key, int size);
	void pressedLeft(bool is_Alt_Key, int size);
	void pressedRight(bool is_Alt_Key, int size);
	void releaseUp(bool is_Alt_Key, int size);
	void releaseDown(bool is_Alt_Key, int size);
	void releaseLeft(bool is_Alt_Key, int size);
	void releaseRight(bool is_Alt_Key, int size);
	void keyPressedCallBack(float dt);
	void keyCallBack(float dt);

	MultiselectSprite* getMultiSelectSprite(){ return multiSelectSprite; };

	void setOpenState(bool state);
	LayerMoveRangeManage* getLayerMoveRangeManage(){ return layerMoveRangeManager; };

	void updateImageVecOrder(std::vector<std::string> layerVec);

	bool judgeNeeedUpdateOutputTexture();
	void setAllyLayerNeedUpdateOutputState(bool state);
private:
	ForeManager();
	virtual ~ForeManager();
	cocos2d::Point startPos;
	cocos2d::Point movedPos;
	//保留存储的数据用于设置父layer的tagname
	QVariant __data;
	bool clear;
	std::vector<ImageSpriteLayer*> _Imagevec;
	Vector<Node*> _copyVec;
	ImageSpriteLayer* targetLayer;
	ImageSprite* targetSpriteChild;
	MultiselectSprite* multiSelectSprite;
	LayerMoveRangeManage* layerMoveRangeManager;

	bool inOpenState;

	enum Key_Type{
		Up,
		Down,
		Left,
		Right,
		None_type
	};
	Key_Type key_type;
	bool is_Alt_Key;
	int move_size;
};

