#include "MisteryBox.h"
#include <iostream>

MisteryBox::MisteryBox()
{
	SetState("running");
	width = 14;
	height = 16;
	//this->v.x = .05;
	//v = Vector(0.05, 0);
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

void MisteryBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//CGameObject::Update(dt, coObjects);

}

void MisteryBox::Render()
{

	//int ani = KOOPAS_ANI_WALKING_LEFT;
	/*if (state == "indie") {

	}*/
	//if (vx == 0) this->state = "indie";
	/*if (vx > 0) this->state = "running-right";
	if (vx < 0) this->state = "running-left";*/

	float width = 0;
	float height = 0;
	animations_set.Get(type).at(state)->Render(p.x, p.y, 255, width, height);

	//SetSize(width, height);


	RenderBoundingBox();
}

void MisteryBox::SetState(string state)
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


void MisteryBox::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;


	right = p.x + width;
	bottom = p.y + height;

}