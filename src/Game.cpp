#include "DXUT.h"
#include "Game.h"


Game::Game(void)
{
	bullet_ = Bullet();
	camera_ = Camera();
	castle_ = Castle();
	BeforeReady();
}


Game::~Game(void)
{
	for( auto monster_it = monster_list_.begin(); monster_it != monster_list_.end(); monster_it++ )
	{
		delete *monster_it;
	}
	monster_list_.clear();	
}

void Game::BeforeReady()
{
	camera_.SetTopView();
	
	bullet_.position_ = castle_.position_;
	bullet_.velocity_.x = 0.f, bullet_.velocity_.y = 0.f, bullet_.velocity_.z = 0.f;

}

void Game::AfterReady()
{

}

void Game::BeforeShoot()
{
	//camera_.position_ = bullet_.position_;
	camera_.Follow(&bullet_.position_);
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
/*
void Game::NewMonster(IDirect3DDevice9* pd3dDevice)
{
	Monster * monster = new Monster(pd3dDevice);
	monster_list_.push_back(monster);
}*/

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

	if(bullet_.didCollide_ == true)
	{
		BulletCollideWithGround();
	}
	

	auto monster_it = monster_list_.begin();
	while (monster_it != monster_list_.end())
	{
		(*monster_it)->Update(dTime);
		if((*monster_it)->state_ == DIE)
		{
			delete *monster_it;
			monster_list_.erase(monster_it++);	
		}
		else 
			++monster_it;
	}
	/*for( auto monster_it = monster_list_.begin(); monster_it != monster_list_.end(); )
	{
		(*monster_it)->Update(dTime);
		if((*monster_it)->state_ == DIE)
		{
			delete *monster_it;
			monster_list_.erase(monster_it);	
		}
		else 
			++monster_it;
	}*/


	//Collision TEST
	for( auto monster_it = monster_list_.begin(); monster_it != monster_list_.end(); monster_it++ )
	{
		float dis = distance((*monster_it)->position_, bullet_.position_);
		//printf("dis : %f\n",dis);
		if( g_game_state_ == SHOOT
			&& bullet_.didCollide_ == false
			&& distance((*monster_it)->position_, bullet_.position_) < (*monster_it)->radius_ + bullet_.radius_ )
		{
			printf("######HITHITHITHITHITHITHITHITHI######\n");
			(*monster_it)->Hit();
			BulletCollideWithMonster();
		}
	}
}
GAME_STATE g_game_state_ = READY;