#pragma once

#define CAMERA_INIT_EYE D3DXVECTOR3( 00.0f, 20.f, -0.0f )
#define CAMERA_INIT_LOOKAT D3DXVECTOR3( 0.0f, 0.f, 0.1f )

class Camera
{
public:
	Camera(void);
	~Camera(void);

	void Update(float dTime);

	void SetTopView();

	void Follow(D3DVECTOR* target, float x, float y);

public:
	D3DXVECTOR3 position_;
	D3DXVECTOR3 lookat_;

private:
	D3DVECTOR* follower_;
};

