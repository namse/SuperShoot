#include "DXUT.h"
#include "Monster.h"
#include <math.h>
/*
Monster::Monster(IDirect3DDevice9* pd3dDevice):CModel(pd3dDevice)
{
	LoadXFile(L"monster.x");
	BeforeMove();
	target_position_ = new D3DXVECTOR3(0.f, 0.f, 0.f);
	float radius = GetBoundingSphereRadius();
	position_.x = 10.f;
	position_.y = 0.f;
	position_.z = 10.f;
}*/

Monster::Monster() : radius_(MONSTER_RADIUS), HP(MONSTER_HP_MAX), hasMesh_(false)
{
	BeforeMove();
	target_position_ = new D3DXVECTOR3(0.f, 0.f, 0.f);
	float random = (float)rand();
	while(random > 1 ) random /= 10.f;
	float rad = random * 3.14f * 2;
	position_.x = 10.f * cos(rad);
	position_.y = 0.f;
	position_.z = 10.f * sin(rad);
}

Monster::~Monster(void)
{
	delete target_position_;
}

void Monster::BeforeMove()
{
	//SetCurrentAnimation(0);
	state_ = MOVE;
}

void Monster::BeforeDie()
{
	state_ = DIE;
}

void Monster::Hit()
{
	HP -= MONSTER_HIT_DAMAGE;
	if(HP <= 0)
	{
		BeforeDie();
	}
}

void Monster::Update(float dTime)
{
	if(state_ == MOVE)
	{
		D3DXVECTOR3 direction = (*target_position_ - position_);
		D3DXVec3Normalize(&direction, &direction );
		position_ += direction * MONSTER_SPEED * dTime;
		//printf("%.1f / %.1f / %.1f\n",position_.x, position_.y, position_.z);
	}
}


void Monster::Render(float dTime)
{
	//CModel::Update(dTime);
	//CModel::Draw();
}