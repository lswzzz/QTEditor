#pragma once
#include "ImageSprite.h"
#include "QString"

class EntityImageSprite : public ImageSprite
{
public:
	static EntityImageSprite* create();
	static EntityImageSprite* create(int index);

	//monster properties
	void setHp(int hp);
	void setMp(int mp);
	void setAttack(int attack);
	void setDefense(int defense);
	void setMagic_Attack(int attack);
	void setMagic_Defense(int defense);
	void setSpeed(float speed);
	void setHardFactor(float factor);
	int getHp(){ return __hp; };
	int getMp(){ return __mp; }
	int getAttack(){ return __attack; }
	int getDefense(){ return __defense; }
	int getMagicAttack(){ return __magic_attack; }
	int getMagicDefense(){ return __magic_defense; }
	float getSpeed(){ return __speed; }
	float getHardFactor(){ return __hard_factor; }
	
	virtual void copySprite(EntityImageSprite* sprite);

	void addDrop(QString id, float odd, int difficulty);
	void resetDropId(int index, QString id);
	void resetDropOdd(int index, float odd);
	void resetDropDifficulty(int index, int difficulty);
	void deleteDrop(QString id, float odd);
	void deleteDrop(int index);
	void setDropGold(int gold);
	void deleteDropList();
	int getDropGold(){ return gold; };
	QString getDropListId(int index){ return _dropVec.at(index)->id; };
	float getDropListOdd(int index){ return _dropVec.at(index)->odds; };
	int getDropListDifficulty(int index){ return _dropVec.at(index)->difficulty; };
	struct dropData{
		QString id;
		float odds;
		int difficulty;
		dropData()
		{
			id = "";
			odds = 0.0f;
			difficulty = 0;
		}
	};
	std::vector<dropData*> getDropList(){ return _dropVec; };

	struct UserAI{
		QString key;
		QString value;
		UserAI(){
			key = "";
			value = "";
		}
	};
	std::vector<UserAI*> getUserAIList(){ return _AI_Vec; };
	QString getUserAIKey(int index){ return _AI_Vec.at(index)->key; };
	QString getUserAIValue(int index){ return _AI_Vec.at(index)->value; };
	void addUserAI(QString key, QString value);
	void resetUserAIKey(int index, QString key);
	void resetUserAIValue(int index, QString value);
	void deleteUserAI(QString key, QString value);
	void deleteUserAI(int index);
	void deleteUserAIList();
	
	void setRangeMinX(int x, bool draw);
	void setRangeMaxX(int x, bool draw);
	void setRangeMinY(int y, bool draw);
	void setRangeMaxY(int y, bool draw);
	void setAIFile(std::string file);
	std::string getAIFile();
	int getRangeMinX();
	int getRangeMaxX();
	int getRangeMinY();
	int getRangeMaxY();

	void setDrawNoVisible(float dt);
private:
	EntityImageSprite();
	virtual ~EntityImageSprite();
	void drawAlertRange();
protected:
	//monsterProperties
	int __hp;
	int __mp;
	int __attack;
	int __defense;
	int __magic_attack;
	int __magic_defense;
	float __speed;
	float __hard_factor;

	//monsterDropProperties;
	int gold;
	std::vector<dropData*> _dropVec;

	std::vector<UserAI*> _AI_Vec;

	//aiProperties
	int rangeMinX;
	int rangeMaxX;
	int rangeMinY;
	int rangeMaxY;
	DrawNode* alertRange;
	std::string aiFile;
};

