#pragma once
#include "event.h"
class EventListener
{
public:
	EventListener(void);
	virtual ~EventListener(void);
	virtual void Notify(event::EventHeader& event) = 0;
};

