#pragma once
#include "cocos2d.h"
#include "QString"

using namespace cocos2d;

enum class SpriteType
{
	NORMAL_BLOCK,
	DYNAMIC_BLOCK,
	MONSTER,
	ROLE,
};

struct animateData{
	std::string animName;
	unsigned char animType;
	unsigned char wrapMode;
	float delay;
	unsigned char numFrames;
	std::vector<int> frameVec;
	void setData(std::string animName, unsigned char animType, unsigned char wrapMode, float delay, unsigned char numFrames, std::vector<int>framevec){
		this->animName = animName;
		this->animType = animType;
		this->wrapMode = wrapMode;
		this->delay = delay;
		this->numFrames = numFrames;
		this->frameVec = framevec;
	};
};

class ImageSprite : public Sprite
{
public:
	//init section
	static ImageSprite* create();
	static ImageSprite* create(int index);
	//virtual bool initWithText(std::string filename, std::string image);
    virtual bool initWithText(Texture2D* texture, cocos2d::Rect rect, std::string png);
	void setParentTag(std::string parentTag);
	std::string getParentTag();
	std::string getTemplateFile();
	std::string getTemplateMD5();
	void setTemplateFile(std::string str, std::string md5);
	//tag section
	void setTagName(std::string tagname);
	std::string getTagName();
	int getImageIndex();

	//animation section
	float getPerTime();
	int getPlayerType();
	void playerAnimation(int type);
	void setPerTime(float perTime);
	void pauseInPerSpriteFrame(int index);
	void setHasAnimation(bool has);
	bool isHasAnimation();
	void setAnimationName(std::string animationname);
	void resetAnimation(Animation* animation);
	//当拷贝精灵的时候不能使用上面的只能使用在已经加入层中的精灵
	void resetAnimation();
	Animation* getAnimation();
	std::string getAnimationName();
	std::vector<int> getAnimationList();

	unsigned char getResType();

	//properties section
	//move Properties
	void translatLine(cocos2d::Point translate);
	void runMove(float time, float delay, float backDelay);
	void setMoveTime(float time);
	void setMoveDelayTime(float time);
	void setMoveBackDelayTime(float time);
	void setStartRunTime(float time);
	void stopMove();
	void pauseMove();
	cocos2d::Point getTranslatDis();
	double getMoveTime();
	int getMoveType();
	float getMoveDelayTime();
	float getMoveBackDelayTime();
	float getStartRunTime();
	void setHasMoveProperties(bool has);
	bool isHasMoveProperties();


	//box properties
    void drawRigid(cocos2d::Point anchr, cocos2d::Size half, int way);
	void showRigid(int type);
	bool getDrawNodeVisible();
	cocos2d::Point getBoxAnchrPos();
	cocos2d::Size getHalf();
	int getBoxWay();
	void setHasBoxProperties(bool has);
	bool isHasBoxProperties();

	//circle properties;
	void setHasCircleProperties(bool has);
	bool isHasCircleProperties();
	cocos2d::Point getCircleAnchrPos();
	float getRadius();
	void drawRigid(cocos2d::Point anchr, float radius);

	//baseProperties
	bool isHasBaseProperties();
	void setHasBaseProperties(bool has);
	//scale rotation flipx flipy anchrx ancrhy
	bool flipx;
	bool flipy;
	void setRotation(double rotation);

	//void DeleteFromParent();
	void setPosition(cocos2d::Point pos);
	cocos2d::Point getStartPos();
	
	SpriteType getType(){ return m_type; };
	void setType(SpriteType type){ m_type = type; };
	

	//entity Properties
	bool isHasEntityProperties();
	void setHasEntityProperties(bool has);
	void setNumGenerate(int num){ numGenerate = num; };
	int getNumGenerate(){ return numGenerate; };
	void setGenerateDelay(float delay){ generateDelay = delay; };
	float getGenerateDelay(){ return generateDelay; };
	void setSleepTime(float time){ sleepTime = time; };
	float getSleepTime(){ return sleepTime; };

	//filter Properties
	bool isHasFilterProperties(){ return hasFilterProperties; };
	void setHasFilterProperties(bool has){ hasFilterProperties = has; };
	int getFilterType(){ return filterType; };
	void setFilterType(int type_){ filterType = type_; };

	//是否是用户自己导出 用户自己导出的话要做额外的设置
	bool isUserOutput;
	virtual void copySprite(ImageSprite* sprite);
	int getCopyCount();
	void setCopyCount(int count);
	
	bool isHasMonsterProperties();
	bool isHasMonsterDropProperties();
	bool isHasAIProperties();
	
	void setHasMonsterDropProperties(bool state);
	void setHasAIProperties(bool state);
	

	bool isHasSimplerTrigger();
	void setHasSimplerTrigger(bool state);
	struct SimpleTri{
		int type;
		unsigned int id;
		SimpleTri(){
			type = 0;
			id = 0;
		}
	};
	std::vector<SimpleTri*> getSimpleTriggerList(){ return _simTri_Vec; };
	unsigned char getSimpleTriType(int index){ return _simTri_Vec.at(index)->type; };
	unsigned int getSimpleTriId(int index){ return _simTri_Vec.at(index)->id; };
	void addSimpleTri(unsigned char type, unsigned int id);
	void resetSimpleTriType(int index, unsigned char type);
	void resetSimpleTriId(int index, unsigned int id);
	void deleteSimpleTri(unsigned char type, unsigned int id);
	void deleteSimpleTri(int index);
	void deleteSimpleTriList();

	unsigned int getId(){ return id; };
	void setId(unsigned int id);
	void setNewId();
	
    cocos2d::Point anchr;
	static std::string imagePrefix;
	static QString imagePrefixQString;
	static QString animationPrefixQString;
	static std::string animationPrefix;
	inline void setCanAutoResetResNameInfo(bool result){ canAutoResetResName = result; };
	void autoResetCanAutoResetResName();
	inline bool getCanAutoResetResNameInfo(){ return canAutoResetResName; };
	static unsigned int getMaxId();

	bool isDontMerge(){ return dontMerge; };
	void setDontMerge(bool dontMerge){ this->dontMerge = dontMerge; };
private:
		
protected:
	ImageSprite();
	virtual ~ImageSprite();
	//实体属性的精灵这个值是false对于一般精灵来说只要精灵创建的名称是程序生成的这个值就是true当true的时候生成的精灵名称是image_index的形式
	bool canAutoResetResName;
	//action
	bool hasAnimation;
	bool hasBaseProperties;
	bool hasMoveProperties;
	bool hasBoxProperties;
	bool hasEntityProperties;
	bool hasCircleProperties;


	bool hasMonsterDropProperties;
	bool hasAIProperties;
	bool hasSimpleTriggerProperties;

	//move section
	float moveTime;
	cocos2d::Point startPos, halfPos;

	//GL坐标
	cocos2d::Point translateDis;
	float moveDelay;
	int direction;
	int moveType;
	float moveBackDelay;
	float startRunTime;

	//box Properties
	int boxway;
	cocos2d::Size half;
	//cocos2d::Point pos1, pos2;
	DrawNode* drawnode;
	Sprite*	  drawNodeParent;
	cocos2d::Point BoxAnchrPos;
	cocos2d::Point CircleAnchrPos;
	float radius;
	

protected:
	SpriteType m_type;
	//tag section
	std::string tagname;
	std::string parentTag;
	int imageIndex;

	//animation section
	Action* runAnimate;
	Action* moveAction;
	std::string animationname;
	float perTime;
	int playerType;
	Animation* animation;
	
	//entity section
	int numGenerate;
	float sleepTime;
	float generateDelay;

	//template 
	std::string templateFile;
	std::string templatemd5;
	
	unsigned int id;
	int copyCount;

	std::vector<SimpleTri*> _simTri_Vec;

	//filter properties
	bool hasFilterProperties;
	int filterType;

	//merge
	bool dontMerge;
protected:
	std::string texturePng;
    cocos2d::Rect rect;
};

