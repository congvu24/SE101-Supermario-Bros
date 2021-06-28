#include "GoldenBrick.h"
#include "Vector.h"
#include "PButton.h"
#include "PlayScence.h"
#include <iostream>


LPDIRECT3DTEXTURE9 GoldenBrick::texture = NULL;
unordered_map<string, LPSPRITE> GoldenBrick::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> GoldenBrick::all_animations; //save all animations
CAnimationSets GoldenBrick::animations_set; //save all the animation sets
json GoldenBrick::data = NULL;

GoldenBrick::GoldenBrick()
{
	SetState("running");
	oldP = Vector(0, 0);
	d = Vector(0, 0);
	v = Vector(0, 0);
	g = Vector(0, 0);
	nx = -1;
	isAllowCollision = true;
	isBlockPlayer = true;
}

void GoldenBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	/*v = v + g * dt;
	if (v.y > 0.35f) v.y = 0.35f;

	CGameObject::Update(dt, coObjects);
	Vector newPos = p + d;
	if (state == "hitting" && newPos.y >= oldP.y) {
		p = oldP;
		SetState("hitted");
	}
	else {
		p = p + d;
	}*/

}


void GoldenBrick::SetState(string state)
{

	if (state == "running") {
	}
	else if (state == "hitting") {
		/*oldP = p;
		v.y = -0.3f;
		g.y = 0.001f;
		ny = -1;*/
	}
	else if (state == "hitted") {
		/*v.y = 0;
		g.y = 0;
		ny = -1;*/
	}


	CGameObject::SetState(state);

}

void GoldenBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}


void GoldenBrick::CollisionWithMario(LPCOLLISIONEVENT e) {

}

void GoldenBrick::OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
	if (type == "P" && state == "running" && event->nx != 0) {
		vector<LPGAMEOBJECT>* effectList = new vector<LPGAMEOBJECT>();
		vector<LPGAMEOBJECT>* allObjects = &((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->objects;

		for (auto i = allObjects->begin(); i != allObjects->end(); i++)
			if (GoldenBrick* obj = dynamic_cast<GoldenBrick*>(*i)) {
				if (obj->type != "P" && obj->state == "running") effectList->push_back(*i);
			}

		this->SetState("hitted");
		LPGAMEOBJECT reward = NULL;
		reward = new PButton();
		reward->ParseFromOwnJson();
		reward->p.y = p.y;
		reward->p.x = p.x + width;
		reward->isAllowCollision = true;
		((PButton*)reward)->listEffect = effectList;
		CGame::GetInstance()->GetCurrentScene()->addObject(reward);
	}
}

void GoldenBrick::Explore() {
	if (type != "P" && state == "running") {
		LPGAMEOBJECT reward = NULL;
		reward = new Coin();
		reward->ParseFromOwnJson();
		reward->p.y = p.y + height;
		reward->p.x = p.x + width;
		reward->SetState("stop");
		CGame::GetInstance()->GetCurrentScene()->addObject(reward);
		this->SetState("hidden");
	}
}