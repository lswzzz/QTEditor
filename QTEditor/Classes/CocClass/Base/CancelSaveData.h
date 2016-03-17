#pragma once
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class CancelSaveData
{
public:
	CancelSaveData();
	~CancelSaveData();
	static CancelSaveData* create();
	void setMaxSize(int size);
	enum openSaveType{
		__Move,
		__DELETE,
	};
	struct saveData{
		vector<Node*> vec;
		vector<cocos2d::Point> posV;
		openSaveType _type;
	};

	void addDeleteData(Vector<Node*> vec);
	void addDeleteData(vector<Node*> vec);
	void addDeleteData(Node* node);
	void addSaveData(saveData* data);
	saveData* getSaveData();
	void addSaveData(vector<Node*> vec);
	void addSaveData(Vector<Node*> vec);
	void addSaveData(Node* node);
	void pressAltZ();

	vector<saveData*> save_V;
	
	int maxSize;
};

