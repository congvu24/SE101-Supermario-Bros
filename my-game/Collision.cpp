#include "Collision.h"
#include "Vector.h"
#include <iostream>


LPDIRECT3DTEXTURE9 Collision::texture = NULL;
unordered_map<string, LPSPRITE> Collision::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> Collision::all_animations; //save all animations
CAnimationSets Collision::animations_set; //save all the animation sets
json Collision::data = NULL;

Collision::Collision()
{
	SetState("running");
	p = Vector(0, 0);
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
	/*float width = 0;
	float height = 0;
	animations_set.Get(type).at(state)->Render(p.x, p.y, 255, width, height);
	RenderBoundingBox();*/
}

void Collision::HandleCollision(LPCOLLISIONEVENT e) {

}