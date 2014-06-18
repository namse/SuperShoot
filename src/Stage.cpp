#include "DXUT.h"
#include "Stage.h"
#include "xmlParser.h"
#include "event_manager.h"

Stage::Stage(void):stageInfo_(NULL),totalStageCount_(-1),killedMonsterInThisStage_(-1),nowStage_(-1),
	appearedMonsterCount(-1)
{
	LoadStageInfo();

	g_event_manager->AddEventListener( event::EVENT_MONSTER_DIE, this);
}


Stage::~Stage(void)
{
	delete[] stageInfo_;
}


void Stage::Notify(event::EventHeader& event)
{
	switch (event.event_type_)//(EventTypes)
	{
	case event::EVENT_NO:
		break;
	case event::EVENT_MONSTER_DIE:
		{
			event::MonsterDieEvent& monster_die_event = (event::MonsterDieEvent &)(event);

			killedMonsterInThisStage_ ++;

			if( stageInfo_[nowStage_].monsterCount >= killedMonsterInThisStage_ )
			{
				nextStage();
			}
		}break;
	}
}

void Stage::nextStage()
{
	if(nowStage_ == -1)
	{
		nowStage_ = 1;
	}
	else
		nowStage_++;

	if(nowStage_ > totalStageCount_)
	{
		finishGame();
	}
	else
	{
		appearedMonsterCount = killedMonsterInThisStage_ = 0;
		regenerateDelay = calculateNextRegenTime();

		event::StageStartEvent next_event;
		next_event.stageNumber = nowStage_;
		next_event.event_sender_p_ = this;
		g_event_manager->Notify(next_event);
	}
}


void Stage::finishGame()
{
	nowStage_ = -1;
	event::FinishGameEvent event;
	event.event_sender_p_ = this;
	g_event_manager->Notify(event);
}


void Stage::Update(float dTime)
{
	if(nowStage_ > 0 && nowStage_ <= totalStageCount_)
	{

		if( appearedMonsterCount < stageInfo_[nowStage_].monsterCount )
		{
			regenerateDelay -= dTime;

			if(regenerateDelay <= 0.f)
			{
				appearedMonsterCount++;

				event::RequestNewMonsterEvent event;
				event.event_sender_p_ = this;
				g_event_manager->Notify(event);

				regenerateDelay = calculateNextRegenTime();
			}
		}

	}
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

float Stage::calculateNextRegenTime()
{
	return stageInfo_[nowStage_].averageRegenerateTime * (1.f + 0.5f * ((float) rand() / (RAND_MAX)) );
}