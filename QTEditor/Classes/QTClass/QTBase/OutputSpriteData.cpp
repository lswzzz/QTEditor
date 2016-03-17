#include "OutputSpriteData.h"


OutputSpriteData::OutputSpriteData()
{
	resName = "";
	animName = "";
	playerNumber = "";
	numInstances = 0;
	numController = 0;
	en_Data.hasProperties = false;
	line_moveData.hasProperties = false;
	rigidData.hasProperties = false;
	monster.hasProperties = false;
	circleData.hasProperties = false;
	monsterDrop.hasProperties = false;
	aiData.hasProperties = false;
	triData.hasProperties = false;
	filData.hasProperties = false;
	dontMerge = false;
}


OutputSpriteData::~OutputSpriteData()
{
	for (int i = 0; i < triData.tri_V.size(); i++){
		delete triData.tri_V.at(i);
	}
	for (int i = 0; i < monsterDrop._vec.size(); i++){
		delete monsterDrop._vec.at(i);
	}
	for (int i = 0; i < aiData.keyV.size(); i++){
		delete aiData.keyV.at(i);
	}
}

bool OutputSpriteData::isEqual(OutputSpriteData* otherdata)
{
	if (this->layerIndex != otherdata->layerIndex){
		return false;
	}
	if (this->dontMerge || otherdata->dontMerge){
		return false;
	}
	if (this->resType != otherdata->resType){
		return false;
	}
	if (resType == 0){
		if (this->resName != otherdata->resName){
			return false;
		}
	}
	else if (resType == 1){
		if (this->animName != otherdata->animName){
			return false;
		}
	}
	else{
		if (this->resName != otherdata->resName){
			return false;
		}
	}
	if (this->numController != otherdata->numController){
		return false;
	}
	if (!en_Data.isEqual(otherdata->en_Data)){
		return false;
	}
	if (!line_moveData.isEqual(otherdata->line_moveData)){
		return false;
	}
	if (!rigidData.isEqual(otherdata->rigidData)){
		return false;
	}
	if (!monster.isEquals(otherdata->monster)){
		return false;
	}
	if (!monsterDrop.isEquals(otherdata->monsterDrop)){
		return false;
	}
	if (!circleData.isEquals(otherdata->circleData)){
		return false;
	}
	if (!aiData.isEquals(otherdata->aiData)){
		return false;
	}
	if (!triData.isEqual(otherdata->triData)){
		return false;
	}
	if (!filData.isEqual(otherdata->filData)){
		return false;
	}
	return true;
}