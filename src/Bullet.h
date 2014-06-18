#pragma once
#define BULLET_SPEED 10.f
#define BULLET_RADIUS 0.5f
class Bullet
{
public:
	Bullet(void);
	~Bullet(void);

	void Update(float dTime);

	void Go(float x, float y);
	bool CheckCollide();

	D3DXVECTOR3 GetPosition()				{ return position_; }
	D3DXVECTOR3* GetPositionPointer()		{ return &position_;	}
	void SetPosition(D3DXVECTOR3 position)	{ position_ = position; }
	void SetVelocity(D3DXVECTOR3 velocity)	{ velocity_ = velocity; }
	bool DidCollide()						{ return didCollide_;	}
	float GetRadius()						{ return radius_;		}

private:
	D3DXVECTOR3 position_;
	D3DXVECTOR3 velocity_;
	float speed_;
	bool didCollide_;
	float radius_;
};

