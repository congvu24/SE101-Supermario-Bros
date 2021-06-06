#include "MiniPortal.h"
#include "Vector.h"
#include <iostream>


LPDIRECT3DTEXTURE9 MiniPortal::texture = NULL;
unordered_map<string, LPSPRITE> MiniPortal::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> MiniPortal::all_animations; //save all animations
CAnimationSets MiniPortal::animations_set; //save all the animation sets
json MiniPortal::data = NULL;

MiniPortal::MiniPortal()
{
	SetState("running");
	p = Vector(0, 0);
}

void MiniPortal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}


void MiniPortal::SetState(string state)
{
	CGameObject::SetState(state);

}

void MiniPortal::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}

void  MiniPortal::Render()
{
	/*float width = 0;
	float height = 0;
	animations_set.Get(type).at(state)->Render(p.x, p.y, 255, width, height);
	RenderBoundingBox();*/
}

void MiniPortal::HandleCollision(LPCOLLISIONEVENT e) {

	/*DebugOut(L"nx %s \n", IntToLPCWSTR(e->nx));
	DebugOut(L"ny %s \n", IntToLPCWSTR(e->ny));*/



	/*if (e->nx == 0 && e->ny < 0) {
		isAllowCollision = true;
	}
	else {
		isAllowCollision = false;
	}*/
}