#include "DXUT.h"
#include "Castle.h"
#include "event_manager.h"

Castle::Castle(void):radius_(CASTLE_RADIUS), HP_(CASTLE_MAX_HP)
{
	position_.x = 0, position_.y = 0, position_.z = 0;
	g_event_manager->AddEventListener(event::EVENT_CASTLE_ATTACK, this);
}


Castle::~Castle(void)
{
}

void Castle::Notify(event::EventHeader& event)
{
	switch (event.event_type_)//(EventTypes)
	{
	case event::EVENT_NO:
		break;
	case event::EVENT_CASTLE_ATTACK:
		{
			HP_--;
			if(HP_ <= 0)
			{
				event::FinishGameEvent finish_event;
				finish_event.event_sender_p_ = this;
				g_event_manager->Notify(finish_event);
			}
		}break;
	}
}