#include "Coin.h"
#include "Vector.h"
#include <iostream>


LPDIRECT3DTEXTURE9 Coin::texture = NULL;
unordered_map<string, LPSPRITE> Coin::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> Coin::all_animations; //save all animations
CAnimationSets Coin::animations_set; //save all the animation sets
json Coin::data = NULL;

Coin::Coin()
{
	SetState("running");
	width = 14;
	height = 16;
	p = Vector(0, 0);
}

void Coin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
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
