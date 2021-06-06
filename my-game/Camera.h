#pragma once
#include <d3dx9.h>
#include "GameObject.h";


class Camera {
public:
	Camera();
	float cam_x;
	float cam_y;
	float move_x = 0;
	float move_y = 0;
	float cam_width = 800;
	float cam_height = 600;
	bool isCameraMoving = false;

	void setCamPos(float x, float y);
	bool isInCam(LPGAMEOBJECT entity, int margin);
	bool isInCamPosition(LPGAMEOBJECT entity, int margin);
	void move(float x, float y);
	bool isInCam(float x, float y, int margin);
	D3DXVECTOR3 calcInCamPosition(float x, float y);

	~Camera();
};