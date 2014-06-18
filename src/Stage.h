#pragma once
#include "event_listener.h"
struct StageInfo{
	int stageNumber;
	int monsterCount;
	int averageRegenerateTime;
};
class Stage : public EventListener
{
public:
	Stage(void);
	~Stage(void);

	void Notify(event::EventHeader& event);
	void Update(float dTime);
	int GetNowStage()			{ return nowStage_; }
	int GetAppearedMonsterCount()	{ return appearedMonsterCount; }
	int GetNowStageMonsterCount()	{ return stageInfo_[nowStage_].monsterCount; }
private:
	void LoadStageInfo();
	void nextStage();
	void finishGame();
	
	float calculateNextRegenTime();

	int totalStageCount_;
	StageInfo *stageInfo_;

	int appearedMonsterCount;
	int killedMonsterInThisStage_;
	int nowStage_;

	float regenerateDelay;

};

