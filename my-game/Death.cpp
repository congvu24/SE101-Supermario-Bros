#include "Death.h"
#include "Vector.h"
#include <iostream>


LPDIRECT3DTEXTURE9 Death::texture = NULL;
unordered_map<string, LPSPRITE> Death::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> Death::all_animations; //save all animations
CAnimationSets Death::animations_set; //save all the animation sets
json Death::data = NULL;
json Death::spriteData = NULL;
Death::Death()
{
	DebugOut(L"init death");
	isBlockPlayer = true;
	isUniversal = true;
}

void Death::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}


void Death::SetState(string state)
{
	CGameObject::SetState(state);
}

void Death::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}

void Death::Render()
{
	RenderBoundingBox();
}

void Death::HandleCollision(LPCOLLISIONEVENT e) {

}