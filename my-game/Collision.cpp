#include "Collision.h"
#include "Vector.h"
#include <iostream>


LPDIRECT3DTEXTURE9 Collision::texture = NULL;
unordered_map<string, LPSPRITE> Collision::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> Collision::all_animations; //save all animations
CAnimationSets Collision::animations_set; //save all the animation sets
json Collision::data = NULL;
json Collision::spriteData = NULL;
Collision::Collision()
{
	isBlockPlayer = true;
	isUniversal = true;
}

void Collision::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}


void Collision::SetState(string state)
{
	CGameObject::SetState(state);

}

void Collision::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}

void  Collision::Render()
{
	RenderBoundingBox();
}

void Collision::HandleCollision(LPCOLLISIONEVENT e) {

}