#pragma once
#include "Camera.h";



Camera::Camera() {

}
void Camera::setCamPos(float x, float y) {
	cam_x = x;
	cam_y = y;
}

bool Camera::isInCam(LPGAMEOBJECT entity, int margin = 100) {

	if (entity->p.x <= cam_width + margin
		&& entity->p.y <= cam_height + margin
		&& entity->p.x >= -margin
		&& entity->p.y >= -margin) {
		return true;
	}
	else return false;
}

bool Camera::isInCam(float x, float y, int margin = 100) {

	if (x <= cam_width + margin
		&& y <= cam_height + margin
		&& x >= -margin
		&& y >= -margin) {
		return true;
	}
	else return false;
}

D3DXVECTOR3 Camera::calcInCamPosition(float x, float y) {
	return D3DXVECTOR3(x - cam_x, y - cam_y, 0);
}
