#pragma once

struct StageInfo{
	int stageNumber;
	int monsterCount;
	int averageRegenerateTime;
};
class Stage
{
public:
	Stage(void);
	~Stage(void);

private:
	void LoadStageInfo();


	int totalStageCount_;
	StageInfo *stageInfo_;

};

