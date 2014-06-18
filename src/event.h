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

struct StartFirstStage : public EventHeader
{
	StartFirstStage(){
		event_type_ = EVENT_START_FIRST_STAGE;
	}
};
/*
struct LoginEvent
	: public EventHeader
{

};

struct MoveEvent
	: public EventHeader
{
	MoveEvent(){
		event_type_ = EVENT_MOVE;
		start_position_ = Point();
		move_velocity_ = Point();
	}
	ObjectID object_move_;
	Point start_position_;
	Point move_velocity_;
};

struct MoveKeyChangeEvent
	: public EventHeader
{
	MoveKeyChangeEvent(){
		event_type_ = EVENT_MOVE_KEY_CHANGE;
		memset(&move_keys_,0,sizeof(move_keys_));
	}
	ObjectID object_id_chnaged_;
	MoveKey move_keys_;	
};

struct HPChangeEvent
	: public EventHeader
{
	HPChangeEvent(){
		event_type_ = EVENT_HP_CHANGE;
	}

	ObjectID object_id_changed_;
	int hp_;
};

struct HitEvent
	: public EventHeader
{
	HitEvent(){
		event_type_ = EVENT_HIT;
	}

	ObjectID victim_id_;
	int damage_;
};

struct AttackKeyChangeEvent
	: public EventHeader
{
	AttackKeyChangeEvent(){
		event_type_ = EVENT_ATTACK_KEY_CHANGE;
	}

	ObjectID object_id_changed_;
	AttackKey attack_key_;
};

struct MoveStopEvent
	: public EventHeader
{
	MoveStopEvent(){
		event_type_ = EVENT_MOVE_STOP;
	}

	ObjectID object_id_stoped;
};*/

}