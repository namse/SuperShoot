#include "DXUT.h"
#include "Game.h"
#include "event_manager.h"

Game::Game(void)
{
	g_event_manager->AddEventListener(event::EVENT_FINISH_GAME, this);
	g_event_manager->AddEventListener(event::EVENT_REQUEST_NEW_MONSTER, this);
	g_event_manager->AddEventListener(event::EVENT_STAGE_START, this);


	bullet_ = Bullet();
	camera_ = Camera();
	castle_ = new Castle();
	BeforeReady();

	event::StartFirstStage event;
	event.event_sender_p_ = this;
	g_event_manager->Notify(event);
}


Game::~Game(void)
{
	for( auto monster_it = monster_list_.begin(); monster_it != monster_list_.end(); monster_it++ )
	{
		delete *monster_it;
	}
	monster_list_.clear();	
	delete castle_;
}


void Game::Notify(event::EventHeader& event)
{
	switch (event.event_type_)//(EventTypes)
	{
	case event::EVENT_NO:
		break;
	case event::EVENT_FINISH_GAME:
		{
			event::FinishGameEvent& finishEvent = (event::FinishGameEvent &)(event);
			printf("Finish Game. bye bye!~ \n");
			PostQuitMessage(WM_QUIT);
		}break;
	
	case event::EVENT_REQUEST_NEW_MONSTER:
		{
			event::RequestNewMonsterEvent& requestEvent = (event::RequestNewMonsterEvent &)(event);
			
			NewMonster();
			
			event::NewMonsterAppearEvent newMonsterEvent;
			newMonsterEvent.event_sender_p_ = this;
			g_event_manager->Notify(newMonsterEvent);

		}break;
	case event::EVENT_STAGE_START:
		{
			event::StageStartEvent& startEvent = (event::StageStartEvent &)(event);
			
			printf("next Stage! stage : %d\n",startEvent.stageNumber);

		}break;
	}
}

void Game::BeforeReady()
{
	camera_.SetTopView();
	
	bullet_.SetPosition(castle_->GetPosition());
	bullet_.SetVelocity(D3DXVECTOR3(0.f, 0.f, 0.f));

}

void Game::AfterReady()
{

}

void Game::BeforeShoot()
{
}

void Game::AfterShoot()
{

}

void Game::ScreenClicked(float x, float y)
{
	switch (g_game_state_)
	{
	case READY:
		{
			StartShoot(x,y);
		}
		break;
	case SHOOT:
		{

		}
		break;
	default:
		break;
	}
}
void Game::StartShoot(float x, float y)
{
	AfterReady();
	BeforeShoot();
	bullet_.Go(x, y);
	camera_.Follow(bullet_.GetPositionPointer(), x, y);
	g_game_state_ = SHOOT;
}

void Game::BulletCollideWithGround()
{
	AfterShoot();
	g_game_state_ = READY;
	BeforeReady();
}

void Game::BulletCollideWithMonster()
{
	AfterShoot();
	g_game_state_ = READY;
	BeforeReady();
}

Monster* Game::NewMonster()
{
	Monster * monster = new Monster();
	monster_list_.push_back(monster);
	return monster;
}

inline float distance(D3DXVECTOR3 A, D3DXVECTOR3 B)
{
	return pow(pow(A.x - B.x, 2) + pow(A.y - B.y, 2) + pow(A.z - B.z , 2) , 0.5);
}
void Game::Update(float dTime)
{
	bullet_.Update(dTime);
	camera_.Update(dTime);
	stage_.Update(dTime);

	if(bullet_.DidCollide() == true)
	{
		BulletCollideWithGround();
	}
	

	auto monster_it = monster_list_.begin();
	while (monster_it != monster_list_.end())
	{
		(*monster_it)->Update(dTime);
		if((*monster_it)->GetState() == DIE)
		{
			event::MonsterDieEvent event;
			event.event_sender_p_ = this;
			g_event_manager->Notify(event);
			delete *monster_it;
			monster_list_.erase(monster_it++);	
		}
		else 
			++monster_it;
	}

	//Collision TEST
	for( auto monster_it = monster_list_.begin(); monster_it != monster_list_.end(); monster_it++ )
	{

		// MONSTER AND BULLET //

		float dis = distance((*monster_it)->GetPosition(), bullet_.GetPosition());
		if( g_game_state_ == SHOOT
			&& bullet_.DidCollide() == false
			&& dis < (*monster_it)->GetRadius() + bullet_.GetRadius() )
		{
			printf("######HITHITHITHITHITHITHITHITHI######\n");
			(*monster_it)->Hit();
			BulletCollideWithMonster();
		}

		// MONSTER AND CASTLE

		dis = distance((*monster_it)->GetPosition(), castle_->GetPosition());
		if( castle_->GetHP() > 0 
			&& dis < (*monster_it)->GetRadius() + castle_->GetRadius() )
		{
			printf("castle Attacked!! \n");
			(*monster_it)->BeforeDie();
			event::CastleAttackEvent attackEvent;
			attackEvent.event_sender_p_ = this;
			g_event_manager->Notify(attackEvent);
		}

	}
}
GAME_STATE g_game_state_ = READY;
Game * g_game = nullptr;