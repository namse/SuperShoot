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


public:
	D3DXVECTOR3 position_;
	D3DXVECTOR3 velocity_;
	float speed_;
	bool didCollide_;
	float radius_;
};

