#pragma once

#define CASTLE_RADIUS 3.f
class Castle
{
public:
	Castle(void);
	~Castle(void);
	D3DVECTOR GetPosition()				{ return position_; }

private:
	D3DVECTOR position_;
	float radius_;
};

