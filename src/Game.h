#pragma once
#include "Bullet.h"
#include "Castle.h"
#include "Camera.h"
#include "Monster.h"
#include <list>
#include "Stage.h"
#include "event_listener.h"
enum GAME_STATE{
	READY,
	SHOOT
};
class Game : public EventListener
{
public:
	Game(void);
	~Game(void);

	void Update(float dTime);

	void StartShoot(float x, float y);
	void ScreenClicked(float x, float y);
	void BulletCollideWithGround();
	void BulletCollideWithMonster();
	Monster* NewMonster();

	
	void Notify(event::EventHeader& event);
private:
	//STATE : READY
	void BeforeReady();
	void AfterReady();
	//STATE : SHOOT
	void BeforeShoot();
	void AfterShoot();

public:
	Camera camera_;
	Bullet bullet_;
	Castle castle_;
	Stage stage_;
	
	std::list<Monster*> monster_list_;
};

extern GAME_STATE g_game_state_;

extern Game * g_game;