#pragma once
#include <d3dx9.h>
#include "GameObject.h";


class Camera {
public:
	Camera();
	float cam_x;
	float cam_y;
	float cam_width  = 800;
	float cam_height = 600;

	void setCamPos(float x, float y);
	bool isInCam(LPGAMEOBJECT entity, int margin);
	bool isInCam(float x, float y, int margin);
	D3DXVECTOR3 calcInCamPosition(float x, float y);

	~Camera();
};