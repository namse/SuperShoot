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

	CDXUTXFileMesh* GetMesh()		{ return &mesh_; }
	bool DidInitMesh()				{ return hasMesh_;}
	void SetInitMeshState(bool state){ hasMesh_ = state; }
	D3DXVECTOR3 GetPosition()		{ return position_; }
	float GetRadius()				{ return radius_; }
	MONSTER_STATE GetState()		{ return state_; }
private:

	//for moving
	D3DXVECTOR3* target_position_;

	D3DXVECTOR3 position_;
	//rotation
	
	CDXUTXFileMesh mesh_;
	bool hasMesh_;

	//Collision
	float radius_;
	int HP;


	//mesh
	MONSTER_STATE state_;
	
};

