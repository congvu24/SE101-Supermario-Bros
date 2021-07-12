#include "MarioSelection.h"
#include <iostream>
#include "Quadtree.h"
#include "MiniPortal.h"
#include "SelectNode.h"
#include "SelectPortal.h"
#include "WorldSelect.h"
#include "Game.h"


MarioSelection::MarioSelection()
{

	width = 24;
	height = 30;
	SetState("idle");
	g = Vector(0, 0);
	right = "node-8";
	left = "";
	down = "";
	up = "";
}


void MarioSelection::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void MarioSelection::Render()
{
	float w = width;
	float h = height;
	animations_set.Get(type).at(state)->Render(p.x, p.y, 255, w, h);
	RenderBoundingBox();
}

void MarioSelection::SetState(string state)
{
	CGameObject::SetState("running");
}

void MarioSelection::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}


void MarioSelection::HandleCollision(LPCOLLISIONEVENT e) {

}


void MarioSelection::ProcessKeyboard(KeyboardEvent kEvent) {

}