#pragma once
#include "Camera.h";



Camera::Camera() {

}
void Camera::setCamPos(float x, float y) {
	//if (x >= cam_x_limit)
		cam_x = x;
	cam_y = y;
	/*if (y >= cam_y_limit)
		cam_y = y;*/
}

bool Camera::isInCamPosition(LPGAMEOBJECT entity, int margin = 100) {

	if (
		entity->p.x < cam_x
		|| entity->p.x + entity->width > cam_x + cam_width
		|| entity->p.y < cam_y
		|| entity->p.y + entity->height > cam_y + cam_height
		) return false;
	return true;
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


void Camera::move(float x, float y) {
	/*setCamPos(cam_x + x, cam_y + y);*/
	move_x = move_x + x;
	move_y = move_y + y;
}

D3DXVECTOR3 Camera::calcInCamPosition(float x, float y) {
	return D3DXVECTOR3(x - cam_x - move_x, y - cam_y - move_y, 0); // +170 here 
}
