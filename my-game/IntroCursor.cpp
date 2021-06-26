#include "IntroCursor.h"
#include <iostream>
#include "Quadtree.h"
#include "MiniPortal.h"
#include "Game.h"


IntroCursor::IntroCursor()
{

	width = 24;
	height = 30;
	SetState("indie");
}


void IntroCursor::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}

void IntroCursor::Render()
{
	float w = width;
	float h = height;
	animations_set.Get(type).at(state)->Render(p.x, p.y, 255, w, h);
	RenderBoundingBox();
}

void IntroCursor::SetState(string state)
{
	CGameObject::SetState(state);

}

void IntroCursor::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}


void IntroCursor::HandleCollision(LPCOLLISIONEVENT e) {

}

void IntroCursor::ProcessKeyboard(KeyboardEvent kEvent) {

}