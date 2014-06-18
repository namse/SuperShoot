#include "DXUT.h"
#include "Bullet.h"
#include "Game.h"


Bullet::Bullet(void):radius_(BULLET_RADIUS)
{
	position_.x = 0, position_.y = 0, position_.z = 0;
	velocity_.x = 0, velocity_.y = 0, velocity_.z = 0;
	speed_ = BULLET_SPEED;
}


Bullet::~Bullet(void)
{
}

void Bullet::Go(float x, float y)
{
	didCollide_ = false;
	float distance = sqrt( (x-position_.x)*(x-position_.x) + (y-position_.y)*(y-position_.y) );
	velocity_.y = (9.8f * distance) / (2.f * speed_);
	velocity_.x = ( ( x - position_.x ) / distance ) * speed_;
	velocity_.z = ( ( y - position_.x ) / distance ) * speed_;
}

void Bullet::Update(float dTime)
{
	if( g_game_state_ == SHOOT )
	{
		velocity_.y += -9.8f * dTime;
		position_.x += velocity_.x * dTime;
		position_.y += velocity_.y * dTime;
		position_.z += velocity_.z * dTime;
		//printf("%.2f / %.2f / %.2f\n",position_.x, position_.y, position_.z);

		if ( CheckCollide() == true )
		{
			didCollide_ = true;
		}
	}
}

bool Bullet::CheckCollide()
{
	if( position_.y <= 0 && velocity_.y <= 0 )
	{
		return true;
	}
	return false;
}