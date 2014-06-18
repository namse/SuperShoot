#pragma once

#define CASTLE_RADIUS 3.f
class Castle
{
public:
	Castle(void);
	~Castle(void);

public:
	D3DVECTOR position_;
	float radius_;
};

