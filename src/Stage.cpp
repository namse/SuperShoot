#include "DXUT.h"
#include "Stage.h"
#include "xmlParser.h"

Stage::Stage(void):stageInfo_(NULL)
{
	LoadStageInfo();
}


Stage::~Stage(void)
{
}

void Stage::LoadStageInfo()
{
	XMLNode xMainNode=XMLNode::openFileHelper(L"stage.xml",L"PMML");

	// this prints "<Condor>":
	XMLNode xNode=xMainNode.getChildNode(L"Stage");
	
	totalStageCount_ = xNode.nChildNode(L"StageInfo");
	
	if(stageInfo_ != NULL)
	{
		delete[] stageInfo_;
	}
	stageInfo_ = new StageInfo[totalStageCount_];
	
	for ( int i = 0 ; i < totalStageCount_; i++)
	{
		stageInfo_[i].stageNumber = _wtoi(xNode.getChildNode(L"StageInfo", i).getAttribute(L"stageNumber"));
		stageInfo_[i].monsterCount = _wtoi(xNode.getChildNode(L"StageInfo", i).getAttribute(L"monsterCount"));
		stageInfo_[i].averageRegenerateTime = _wtoi(xNode.getChildNode(L"StageInfo", i).getAttribute(L"averageRegenerateTime"));
	}

	return;
}