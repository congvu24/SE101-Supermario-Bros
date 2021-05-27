#include "Coin.h"
#include <iostream>

Coin::Coin()
{
	SetState("running");
	width = 14;
	height = 16;
}

void Coin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//CGameObject::Update(dt, coObjects);

}

void Coin::Render()
{
	float width = 0;
	float height = 0;
	animations_set.Get(type).at(state)->Render(p.x, p.y, 255, width, height);
	RenderBoundingBox();
}

void Coin::SetState(string state)
{
	CGameObject::SetState(state);

}

void Coin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}