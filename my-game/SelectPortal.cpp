#include "SelectPortal.h"
#include "Vector.h"
#include <iostream>


LPDIRECT3DTEXTURE9 SelectPortal::texture = NULL;
unordered_map<string, LPSPRITE> SelectPortal::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> SelectPortal::all_animations; //save all animations
CAnimationSets SelectPortal::animations_set; //save all the animation sets
json SelectPortal::data = NULL;
json SelectPortal::spriteData = NULL;
SelectPortal::SelectPortal()
{
	SetState("running");
}

void SelectPortal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}


void SelectPortal::SetState(string state)
{
	CGameObject::SetState(state);

}

void SelectPortal::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}

void  SelectPortal::Render()
{
	/*float width = 0;
	float height = 0;
	animations_set.Get(type).at(state)->Render(p.x, p.y, 255, width, height);
	RenderBoundingBox();*/
}

void SelectPortal::HandleCollision(LPCOLLISIONEVENT e) {

	/*DebugOut(L"nx %s \n", IntToLPCWSTR(e->nx));
	DebugOut(L"ny %s \n", IntToLPCWSTR(e->ny));*/



	/*if (e->nx == 0 && e->ny < 0) {
		isAllowCollision = true;
	}
	else {
		isAllowCollision = false;
	}*/
}