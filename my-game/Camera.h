#pragma once
#include <d3dx9.h>
#include <limits>
#include "GameObject.h"
#include "Config.h"

class Camera {
public:
	Camera();
	float cam_x;
	float cam_y;
	float camera_default_left;
	float camera_default_top;
	float cam_left_limit;
	float cam_top_limit;
	float cam_right_limit = std::numeric_limits<float>::infinity();
	float cam_bottom_limit = std::numeric_limits<float>::infinity();
	float move_x = 0;
	float move_y = 0;
	float cam_width = SCREEN_WIDTH;
	float cam_height = SCREEN_HEIGHT;
	bool isCameraMoving = false;

	void setCamPos(float x, float y);
	bool isInCam(LPGAMEOBJECT entity, int margin);
	bool isInCamPosition(LPGAMEOBJECT entity, int margin);
	void move(float x, float y);
	bool isInCam(float x, float y, int margin);
	D3DXVECTOR3 calcInCamPosition(float x, float y);

	~Camera();
};