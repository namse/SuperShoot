#include "DXUT.h"
#include "event_manager.h"

EventManager::EventManager(void)
{
}


EventManager::~EventManager(void)
{
}


void EventManager::AddEventListener(EventTypes event_type, EventListener* pListener)
{
	event_listener_list.insert( std::pair<EventTypes, EventListener*>(event_type, pListener) );
}
void EventManager::RemoveEventListener(EventTypes event_type, EventListener* pListener)
{	
	typedef std::multimap<EventTypes, EventListener*>::iterator iterator;
	std::pair<iterator, iterator> iterpair = event_listener_list.equal_range(event_type);

	iterator it = iterpair.first;
	for (; it != iterpair.second; ++it) {
		if (it->second == pListener) { 
			event_listener_list.erase(it);
			break;
		}
	}
	
}
void EventManager::RemoveEventListener(EventListener* pListener)
{
	for (auto it = event_listener_list.begin(); it != event_listener_list.end(); ) {
		if (it->second == pListener)
			it = event_listener_list.erase(it);
		else
			++it;
	}
}

void EventManager::Notify(EventHeader& event)
{
	typedef std::multimap<EventTypes, EventListener *>::iterator iterator;
	std::pair<iterator, iterator> iterpair = event_listener_list.equal_range(event.event_type_);

	iterator it = iterpair.first;
	for (; it != iterpair.second; ++it) {
		it->second->Notify(event);
	}
}

EventManager *g_event_manager = nullptr;