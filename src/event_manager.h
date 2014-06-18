#pragma once
#include "event.h"
#include "event_listener.h"
#include <map>
class EventManager
{
public:
	EventManager(void);
	~EventManager(void);

	void AddEventListener(event::EventTypes event_type, EventListener* pListener);
	void RemoveEventListener(event::EventTypes event_type, EventListener* pListener);
	void RemoveEventListener(EventListener* object_id);
	void Notify(event::EventHeader& event);



private:
	std::multimap<event::EventTypes, EventListener*> event_listener_list;


};


extern EventManager* g_event_manager;