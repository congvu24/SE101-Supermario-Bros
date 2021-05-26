#include "Collision.h"
#include <iostream>

Collision::Collision()
{

	int id = 0;
	d = Vector(0, 0);
	g = Vector(0, 0);
	v = Vector(0, 0);
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

void Collision::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//CGameObject::Update(dt, coObjects);

}

void Collision::Render()
{

	//int ani = KOOPAS_ANI_WALKING_LEFT;
	/*if (state == "indie") {

	}*/
	//if (vx == 0) this->state = "indie";
	/*if (vx > 0) this->state = "running-right";
	if (vx < 0) this->state = "running-left";*/

	float width = 0;
	float height = 0;
	/*animations_set.Get(type).at(state)->Render(p.x, p.y, 255, width, height);*/

	//SetSize(width, height);


	RenderBoundingBox();
}

void Collision::SetState(string state)
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

void Collision::ParseFromJson(json data) {


	int id = stoi(string(data["id"])); //object id;


	string type = to_string(data["type"]); //object type;

	float x = data["x"]; //object x;
	float y = data["y"]; //object y;

	LPCWSTR sprite = ToLPCWSTR(string(data["sprite"]));
	LPCWSTR texture = ToLPCWSTR(string(data["texture"]));
	LPCWSTR animation = ToLPCWSTR(string(data["animation"]));

	// set inital position
	this->id = id;
	this->type = type;
	this->p.x = x;
	this->p.y = y;
	//
	D3DCOLOR transcolor;
	SetTexture(texture, D3DCOLOR_XRGB(255, 0, 255));
	ParseSpriteFromJson(sprite);

	ParseAnimationFromJson(animation);

	// set active animation set;
	SetActiveAnimationSet(type);
}

void Collision::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;


	right = p.x + width;
	bottom = p.y + height;

	//right = x + MARIO_SMALL_BBOX_WIDTH;
	//bottom = y + MARIO_SMALL_BBOX_HEIGHT;
}