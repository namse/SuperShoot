#pragma once
#include <memory>

namespace event
{
	enum EventTypes
	{
		EVENT_NO	= 0,

		EVENT_REQUEST_NEW_MONSTER,
		EVENT_NEW_MONSTER_APPEAR,
		EVENT_MONSTER_DIE,
		EVENT_START_FIRST_STAGE,

		EVENT_STAGE_START,
		EVENT_FINISH_GAME,

		EVENT_CASTLE_ATTACK,
	} ;

	struct EventHeader
	{
		EventTypes event_type_;
		void* event_sender_p_;
	};

	struct RequestNewMonsterEvent : public EventHeader
	{
		RequestNewMonsterEvent(){
			event_type_ = EVENT_REQUEST_NEW_MONSTER;

		}
	};

	struct NewMonsterAppearEvent : public EventHeader
	{
		NewMonsterAppearEvent(){
			event_type_ = EVENT_NEW_MONSTER_APPEAR;
		}
	};

	struct MonsterDieEvent : public EventHeader
	{
		MonsterDieEvent(){
			event_type_ = EVENT_MONSTER_DIE;
		}
	};

	struct StageStartEvent : public EventHeader
	{
		StageStartEvent(){
			event_type_ = EVENT_STAGE_START;
			stageNumber = -1;
		}
		int stageNumber;
	};

	struct FinishGameEvent : public EventHeader
	{
		FinishGameEvent(){
			event_type_ = EVENT_FINISH_GAME;
		}
	};

	struct CastleAttackEvent : public EventHeader
	{
		CastleAttackEvent(){
			event_type_ = EVENT_CASTLE_ATTACK;
		}
	};


	struct StartFirstStage : public EventHeader
	{
		StartFirstStage(){
			event_type_ = EVENT_START_FIRST_STAGE;
		}
	};

}