#include "PButton.h"
#include "Vector.h"
#include "Test.h"
#include "GoldenBrick.h"
#include <iostream>


LPDIRECT3DTEXTURE9 PButton::texture = NULL;
unordered_map<string, LPSPRITE> PButton::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> PButton::all_animations; //save all animations
CAnimationSets PButton::animations_set; //save all the animation sets
json PButton::data = NULL;

PButton::PButton()
{
	SetState("running");
	isBlockPlayer = true;
	isAllowCollision = true;
}

void PButton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}


void PButton::SetState(string state)
{
	CGameObject::SetState(state);
}

void PButton::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}


void PButton::HandleCollision(LPCOLLISIONEVENT e) {

}

void PButton::OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
	if (state == "running" && event->ny < 0) {
		this->SetState("hitted");
		for (auto i = listEffect->begin(); i != listEffect->end(); i++)
			if (GoldenBrick* obj = dynamic_cast<GoldenBrick*>(*i)) {
				obj->Explore();
			}
	}
}