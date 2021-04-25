#include "Enemy.h"
#include <iostream>

Enemy::Enemy()
{
	SetState("indie");
	this->vx = .05;
	//this->dx = 1;
	//this->dy = 1;
}

//void Test::GetBoundingBox(float& left, float& top, float& right, float& bottom)
//{
//	/*left = x;
//	top = y;
//	right = x + KOOPAS_BBOX_WIDTH;
//
//	if (state == KOOPAS_STATE_DIE)
//		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
//	else
//		bottom = y + KOOPAS_BBOX_HEIGHT;*/
//}

void Enemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	/*if (state == "running-right") vx = 0.5f;
	if (state == "running-left") vx = -0.5f;*/

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
	}
}

void Enemy::Render()
{

	//int ani = KOOPAS_ANI_WALKING_LEFT;
	/*if (state == "indie") {

	}*/
	//if (vx == 0) this->state = "indie";
	/*if (vx > 0) this->state = "running-right";
	if (vx < 0) this->state = "running-left";*/

	int width = 0;
	int height = 0;
	animations_set.Get(type).at(state)->Render(x, y, 255, width, height);

	SetSize(width, height);


	RenderBoundingBox();
}

void Enemy::SetState(string state)
{
	CGameObject::SetState(state);

	//if (state == "running-right") {
	//	vx = 0.15f;
	//	nx = 1;
	//}
	//else if (state == "running-left") {
	//	vx = -0.15f;
	//	nx = -1;
	//}
	//else if (state == "indie") {
	//	vx = 0;

	//}


	/*CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_WALKING:
		vx = KOOPAS_WALKING_SPEED;
	}*/

}

void Enemy::ParseFromJson(json data) {


	int id = stoi(string(data["id"])); //object id;


	string type = to_string(data["type"]); //object type;

	float x = data["x"]; //object x;
	float y = data["y"]; //object y;

	LPCWSTR sprite = ToLPCWSTR(string(data["sprite"]));
	LPCWSTR texture = ToLPCWSTR(string(data["texture"]));
	LPCWSTR animation = ToLPCWSTR(string(data["animation"]));

	// set inital position
	id = id;
	this->type = type;
	this->x = x;
	this->y = y;
	//
	D3DCOLOR transcolor;
	SetTexture(texture, D3DCOLOR_XRGB(255, 0, 255));
	ParseSpriteFromJson(sprite);

	ParseAnimationFromJson(animation);

	// set active animation set;
	SetActiveAnimationSet(type);
}

void Enemy::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (type == "1")
	{
		right = x + width;
		bottom = y + height;
	}
	else
	{
		right = x + width;
		bottom = y + height;

		//right = x + MARIO_SMALL_BBOX_WIDTH;
		//bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}