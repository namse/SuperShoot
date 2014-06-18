#pragma once
#define MONSTER_SPEED 1.f
#include "CModel.h"
#include "SDKmesh.h"

#define MONSTER_RADIUS 0.5f
#define MONSTER_HP_MAX 2
#define MONSTER_HIT_DAMAGE 1
#include "event_listener.h"
enum MONSTER_STATE
{
	IDLE,
	MOVE,
	DIE,
};
class Monster
{
public:
	//Monster(IDirect3DDevice9* pd3dDevice);
	Monster(void);
	~Monster(void);

	void Update(float dTime);
	void Render(float dTime);
	void BeforeMove();
	void Hit();
	void BeforeDie();
	D3DXVECTOR3 position_;
	//rotation
	
	CDXUTXFileMesh mesh_;
	bool hasMesh;

	//Collision
	float radius_;
	int HP;


	//mesh
	MONSTER_STATE state_;
private:

	//for moving
	D3DXVECTOR3* target_position_;
	
};

