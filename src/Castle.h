#pragma once
#include "event_listener.h"
#define CASTLE_RADIUS 1.f
#define CASTLE_MAX_HP 3
class Castle : EventListener
{
public:
	Castle(void);
	~Castle(void);
	D3DVECTOR GetPosition()				{ return position_; }
	int GetHP()							{ return HP_;		}
	float GetRadius()					{ return radius_;	}
	void Notify(event::EventHeader& event);

private:
	D3DVECTOR position_;
	float radius_;
	int HP_;
};

