#include "Test.h"
#include <iostream>

Test::Test()
{
	SetState("indie-front");
}

void Test::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	/*left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;*/
}

void Test::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 

	/*x += dx;
	y += dy;

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
	}*/
}

void Test::Render()
{

	/*int ani = KOOPAS_ANI_WALKING_LEFT;
	if (state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_DIE;
	}
	else if (vx > 0) ani = KOOPAS_ANI_WALKING_RIGHT;
	else if (vx <= 0) ani = KOOPAS_ANI_WALKING_LEFT;

	animation_set->at(ani)->Render(x, y);*/

	animations_set.Get(type).at(state)->Render(x, y);


	RenderBoundingBox();
}

void Test::SetState(string state)
{
	CGameObject::SetState(state);
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

void Test::ParseFromJson(json data) {


	int id = stoi(string(data["id"])); //object id;


	string type = to_string(data["type"]); //object type;

	int x = data["x"]; //object x;
	int y = data["y"]; //object y;

	LPCWSTR sprite = ToLPCWSTR(data["sprite"].dump());
	LPCWSTR texture = ToLPCWSTR(data["texture"].dump());
	LPCWSTR animation = ToLPCWSTR(data["animation"].dump());

	// set inital position
	id = id;
	this->type = type;
	x = x;
	y = y;
	//
	D3DCOLOR transcolor;
	SetTexture(texture, D3DCOLOR_XRGB(255, 0, 255));
	ParseSpriteFromJson(sprite);

	ParseAnimationFromJson(animation);

	// set active animation set;
	SetActiveAnimationSet(type);


}