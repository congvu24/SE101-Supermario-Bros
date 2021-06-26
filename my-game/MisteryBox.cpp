#include "MisteryBox.h"
#include "Mushroom.h"
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
	oldP = Vector(0, 0);
	d = Vector(0, 0);
	v = Vector(0, 0);
	g = Vector(0, 0);
	nx = -1;
	isAllowCollision = false;
}

void MisteryBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	v = v + g * dt;
	if (v.y > 0.35f) v.y = 0.35f;

	CGameObject::Update(dt, coObjects);
	Vector newPos = p + d;
	if (state == "hitting" && newPos.y >= oldP.y) {
		p = oldP;
		SetState("hitted");
	}
	else {
		p = p + d;
	}

}


void MisteryBox::SetState(string state)
{

	if (state == "running") {
	}
	else if (state == "hitting") {
		oldP = p;
		v.y = -0.3f;
		g.y = 0.001f;
		ny = -1;
	}
	else if (state == "hitted") {
		v.y = 0;
		g.y = 0;
		ny = -1;
	}


	CGameObject::SetState(state);

}

void MisteryBox::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}


void MisteryBox::CollisionWithMario(LPCOLLISIONEVENT e) {
	if (isHitted == false && e->ny >= 0 && e->nx == 0) {
		this->SetState("hitting");
		isHitted = true;
		LPGAMEOBJECT reward = NULL;
		if (name == "QuestionBox_Coin") {
			reward = new Coin();
			reward->ParseFromOwnJson();
			reward->p.y = p.y - 100;
			reward->p.x = p.x;
			reward->isAllowCollision = false;
			CGame::GetInstance()->GetCurrentScene()->addObject(reward);
			reward->SetState("fromMisteryBox");
		}
		else if (name == "QuestionBox_Mushroom") {
			reward = new Mushroom();
			reward->ParseFromOwnJson();
			reward->p.y = p.y + height / 2;
			reward->p.x = p.x;
			reward->name = "Mushroom";
			reward->isAllowCollision = true;
			CGame::GetInstance()->GetCurrentScene()->addObject(reward);
			reward->SetState("fromMisteryBox");
		}
		else {
			reward = new Leaf();
			reward->ParseFromOwnJson();
			reward->p.y = p.y - 100;
			reward->p.x = p.x;
			reward->name = "Leaf";
			reward->isAllowCollision = true;
			CGame::GetInstance()->GetCurrentScene()->addObject(reward);
			reward->SetState("fromMisteryBox");
		}
	}

}