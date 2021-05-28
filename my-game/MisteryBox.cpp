#include "MisteryBox.h"
#include "Vector.h"
#include <iostream>


LPDIRECT3DTEXTURE9 MisteryBox::texture = NULL;
unordered_map<string, LPSPRITE> MisteryBox::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> MisteryBox::all_animations; //save all animations
CAnimationSets MisteryBox::animations_set; //save all the animation sets
json MisteryBox::data = NULL;

MisteryBox::MisteryBox()
{
	SetState("running");
	width = 14;
	height = 16;
	p = Vector(0, 0);
}

void MisteryBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}


void MisteryBox::SetState(string state)
{
	CGameObject::SetState(state);

}

void MisteryBox::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}
