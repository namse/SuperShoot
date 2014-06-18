#include "DXUT.h"
#include "Camera.h"
#include "Game.h"

Camera::Camera(void)
{
}


Camera::~Camera(void)
{
}

void Camera::SetTopView()
{
	//position_.x = 0.f,	position_.y = 100.f,	position_.z = -0.f;
	//lookat_.x = 0.f,	lookat_.y = 0.f,	lookat_.z = 0.001f;
	position_ = CAMERA_INIT_EYE;
	lookat_ = CAMERA_INIT_LOOKAT;
}

void Camera::Follow(D3DVECTOR* target, float x, float y)
{
	follower_ = target;
	position_ = *follower_;
	lookat_ = D3DXVECTOR3(x,0,y);//*follower_;
}

void Camera::Update(float dTime)
{
	if(g_game_state_ == SHOOT)
	{
		/*float distance = sqrt(	(follower_->x - position_.x) * (follower_->x - position_.x) +
			(follower_->y - position_.y) * (follower_->y - position_.y) +
			(follower_->z - position_.z) * (follower_->z - position_.z)	);*/
		position_.x = (position_.x + follower_->x)/2.f;
		position_.y = (position_.y + follower_->y)/2.f;
		position_.z = (position_.z + follower_->z)/2.f;
		//lookat_ = *follower_;
	}
}