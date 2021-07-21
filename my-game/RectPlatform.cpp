#include "RectPlatform.h"
#include "Vector.h"
#include <iostream>


LPDIRECT3DTEXTURE9 RectPlatform::texture = NULL;
unordered_map<string, LPSPRITE> RectPlatform::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> RectPlatform::all_animations; //save all animations
CAnimationSets RectPlatform::animations_set; //save all the animation sets
json RectPlatform::data = NULL;
json RectPlatform::spriteData = NULL;
RectPlatform::RectPlatform()
{
	isUniversal = true;
}

void RectPlatform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}


void RectPlatform::SetState(string state)
{
	CGameObject::SetState(state);

}

void RectPlatform::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}

void  RectPlatform::Render()
{
	/*float width = 0;
	float height = 0;
	animations_set.Get(type).at(state)->Render(p.x, p.y, 255, width, height);
	RenderBoundingBox();*/
}

void RectPlatform::HandleCollision(LPCOLLISIONEVENT e) {

	/*DebugOut(L"nx %s \n", IntToLPCWSTR(e->nx));
	DebugOut(L"ny %s \n", IntToLPCWSTR(e->ny));*/



	/*if (e->nx == 0 && e->ny < 0) {
		isAllowCollision = true;
	}
	else {
		isAllowCollision = false;
	}*/
}