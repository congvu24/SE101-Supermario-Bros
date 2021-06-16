#include "IntroText.h"
#include "Vector.h"
#include <iostream>


LPDIRECT3DTEXTURE9 IntroText::texture = NULL;
unordered_map<string, LPSPRITE> IntroText::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> IntroText::all_animations; //save all animations
CAnimationSets IntroText::animations_set; //save all the animation sets
json IntroText::data = NULL;

IntroText::IntroText()
{
	SetState("indie");
	width = 14;
	height = 16;
	p = Vector(0, 0);

}

void IntroText::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}


void IntroText::SetState(string state)
{
	CGameObject::SetState(state);

}

void IntroText::HandleCollision(LPCOLLISIONEVENT e) {
}