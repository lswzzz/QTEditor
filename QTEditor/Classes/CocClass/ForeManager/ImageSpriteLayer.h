#pragma once
#include "cocos2d.h"
#include "ImageSprite.h"
#include "EntityImageSprite.h"
#include "OutputSpriteData.h"
#include "Global.h"
#include "ImageTextureManager.h"
#include "LayerAnimationCache.h"

using namespace cocos2d;

class ResInfo {
public:
	ResInfo() {
		refCount = 0;
		lastRefCount = 0;
		isExteral = false;
		isChanged = false;
	}
	virtual ~ResInfo() {}

	bool isExteral;
	bool isChanged;
	std::string path;
	unsigned int refCount;
	unsigned int lastRefCount;
};

class AnimationInfo{
public:
	AnimationInfo(){
		isChanged = false;
		refCount = 0;
		lastRefCount = 0;
	}
	bool isChanged;
	std::vector<int> animationList;
	std::string animationname;
	unsigned int refCount;
	unsigned int lastRefCount;
};

class ImageSpriteLayer : public Layer
{
public:
	static ImageSpriteLayer* create(std::string name, bool inOpenState);
	virtual bool init();

	virtual void update(float dt);
	void setTargetSpriteByPoint(cocos2d::Point pos);

	ImageSprite* addImageSprite(cocos2d::Point pos, std::string listFile, std::string tagname, Image_Type type_);
	ImageSprite* addSpriteInOpenFile(cocos2d::Point pos, std::string realFileName, std::string tagname, Image_Type type__, std::string animationname = "");
	EntityImageSprite* AddImageSpriteAddToMapDefaultInfo(int imageIndex, Point pos, std::string tagname, Image_Type type_, std::string animationname = "");
	void		resetImageSpriteDefaultBaseInfo(ImageSprite* sprite, Point pos, std::string tagname, Image_Type type_);
	bool		 resetImageSpriteTexture(ImageSprite* sprite);
	bool		resetImageSpriteBatchInfo(ImageSprite* sprite);
	bool		resetImageSpriteSpriteFrame(ImageSprite* sprite);
	bool		resetImageSpriteFrameInfo(ImageSprite* sprite);
	bool		resetImageSpriteFrameInfoInOpen(ImageSprite* sprite, std::string animationname);

	void		addOtherLayerCopySprites(std::vector<ImageSprite*> spriteVec, bool addToTemplate);
private:
	void		addOtherLayerSpriteToSpriteMap(std::map<int, std::vector<ImageSprite*>*>& spriteMap, ImageSprite* sprite);
	bool		addOtherLayerSpriteToLayerMap(std::map<int, std::vector<ImageSprite*>*>& spriteMap, Image_Type type_);
	void		deleteSpriteMap(std::map<int, std::vector<ImageSprite*>*>& spriteMap);
public:
	void		AsynPackerImageInOpenFile();
	void		texturePackerCommand(bool showPackerLabel);

	void MoveSprite(cocos2d::Point pos);
	void setStartPos(cocos2d::Point pos);
	//右键删除图片功能
	void removeSprite(cocos2d::Point pos);
	void removeSpriteRemoveFromMap(ImageSprite* sprite);
	void removeAllSpriteAndUpdateChange();

	void setTagName(std::string name);
	std::string getTagName();
	Vector<Node*>	getAllSpriteNode();
	ImageSprite*	getSpriteFromTagName(std::string tagname);
	void			removeSpriteFromTagName(std::string tagname);
	void			subResRefFromSprite(ImageSprite* sprite);

	void			changeSpriteTag(std::string oldtag, std::string newtag);
	void			changeSpriteZOrderFromImageLayer(std::string tagname, int z_order);
	void			setTargetSprite(std::string tagname);
	//not contains role image
	std::vector<std::string> getAllResourceImage();
	std::vector<animateData> getAllAnimateData();
	std::vector<std::string> getAllResourceImageContainsExteral();
	void setImageIndex();
	void setisSportState(bool state);
	bool isSportState();
	
	cocos2d::Point* getLayerRange();
	void			changeAllSpriteToBatch(int count);
	void			loadingTextureCallback(Texture2D *texture);
	void			NeedUpdateSprite(unsigned int* data);
	std::vector<OutputSpriteData*> getOutputSpriteData();
	std::vector<OutputSpriteData*> getOutputSpriteData(std::vector<ImageSpriteLayer*> layerVec);
	void			releaseOutputSpriteData();

	void			setLock(bool lock);
	bool			isLock();
	bool			isDontOutput();
	void			setDontOutputState(bool dontoutput);
	int				getStaticIndex();
	void			setStaticIndex(int index);

	void			setMoveScaleX(float scale);
	void			setMoveScaleY(float scale);
	void			setSceneScaleFluence(float scale);
	float			getMoveScaleX();
	float			getMoveScaleY();
	float			getSceneScaleFluence();
	int				getFilterType();
	void			setFilterType(int type_);
	bool			addToMap(std::string imagePath, bool isExteral);
	bool			addImageToMap(std::string imagePath);
	bool			addExteralToMap(std::string imagePath);
	bool			addAnimationToMap(std::string animationname, std::vector<int> animationList);
	void			addCopySpriteCurLayer(ImageSprite* sprite);
	void			removeFromMap(std::string imagePath);
	void			removeFromAnimatiomMap(std::string animation);
	bool			containsInImageMap(const std::string& listImagePath);
	bool			containsInAnimationMap(const std::string& animatonname);
	void			createTextureManager(QString filename, int count);
	void			resetAnimationCache();
	void			setOpenState(bool state);

	void updateChanged();
	void addSpriteOver();
	void removeSpriteOver();

	bool isSameProperties(ImageSpriteLayer* layer);
	bool hasRange();
	float getRangeLeftTopX();
	float getRangeLeftTopY();
	float getRangeRightBottomX();
	float getRangeRightBottomY();

	unsigned int generateThreadTag();
	void setCurrentTag(unsigned int tag);

	bool containsInUnLoadVec(int index);
	void addToUnLoadVec(int index);
	void clearLoadedImageIndex();
	
	void resetLayerIndex();
	inline void setLayerIndex(int index){ layerIndex = index; };
	inline int getLayerIndex(){ return layerIndex; };
	
	bool isNotContainsAnyRes();

	//判断当前的层在导出的时候是否需要重新导出如果所有的层的这个值没有被更新的话那么导出就不需要重新打包纹理了
	inline void setNeedOutputUpdate(bool result){ needOutputUpdate = result; };
	inline bool getNeedOutputUpdateState(){return needOutputUpdate;};

	inline LayerAnimationCache* getAnimationCache(){ return animationCache; };
	std::vector<int> getAnimationSpriteFrameList(std::string animationname);
	inline bool isHasAnimationSprite(){ return animationResMap.size() == 0 ? false : true; };
private:
	//这个值是layerManager管理的层顺序的下表
	int layerIndex;
	ImageSpriteLayer();
	virtual ~ImageSpriteLayer();
	std::map<std::string, int> imageIndexMap;
	int texturecount;
	std::vector<OutputSpriteData*> vecOutputData;
	cocos2d::Point start, end;
	ImageSprite* targetSprite;
	std::string tagname;
	bool sport_state;
	std::string outputDir;
	bool updateSprite;
	bool __lock;
	bool isdontoutput;
	int static_index;
	bool inPackerAsyn;

	bool inOpenState;
	bool updateAnchr;

	float moveScaleX;
	float moveScaleY;
	float sceneScaleFluence;
	int filterType;
	QString m_filename;
	ImageTextureManager* textureManager;
	LayerAnimationCache* animationCache;
	//imageMap 保存的是Rsources图片
	std::map<std::string, ResInfo*> resMap;
	//保存的是Exteral图片的路径
	std::map<std::string, ResInfo*> roleMap;
	std::map<std::string, AnimationInfo*> animationResMap;
	
	std::vector<int>			unAddImageVec;
	std::vector<ResInfo*> _changedResInfos;
	std::vector<AnimationInfo*> _changeAnimationInfos;
	std::mutex m_Outputmutex;
	std::recursive_mutex tagmutex;
	unsigned int topTag;
	unsigned int currentTag;
	bool needOutputUpdate;
	void updateCurTag();
};

