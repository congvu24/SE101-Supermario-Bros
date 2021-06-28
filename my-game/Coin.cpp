#include "Coin.h"
#include "Vector.h"
#include "Test.h"
#include <iostream>


LPDIRECT3DTEXTURE9 Coin::texture = NULL;
unordered_map<string, LPSPRITE> Coin::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> Coin::all_animations; //save all animations
CAnimationSets Coin::animations_set; //save all the animation sets
json Coin::data = NULL;

Coin::Coin()
{
	SetState("running");
	isBlockPlayer = false;
	isAllowCollision = false;
}

void Coin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//if (state != "hidden") {
	v = v + g * dt;
	if (v.y > 1.0f) v.y = 1.0f;
	CGameObject::Update(dt, coObjects);
	if (state == "fromMisteryBox") {
		Vector newPos = p + d;
		if (newPos.y > oldP.y) {
			p = oldP;
			SetState("hidden");
		}
		else {
			p = newPos;
		}
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;


	coEvents.clear();
	for (auto i = coObjects->begin(); i != coObjects->end(); i++)
	{

		if (Test* obj = dynamic_cast<Test*>((*i))) {
			LPCOLLISIONEVENT e = SweptAABBEx(*i);

			if (e->t > 0 && e->t <= 1.0f) {
				coEvents.push_back(e);
			}
			else {
				delete e;
			}
		}
	}

	float min_tx, min_ty, nx = 0, ny;
	float rdx = 0;
	float rdy = 0;

	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

	if (rdx != 0 && rdx != d.x)
		p.x += nx * abs(rdx);

	// block every object first!

	for (UINT i = 0; i < coEventsResult.size(); i++) {

		if (Test* obj = dynamic_cast<Test*>(coEventsResult[i]->obj)) {
			this->SetState("hidden");
		}

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	coEvents.clear();
	coEventsResult.clear();
}

 void  Coin::Render()
{
	float w = width;
	float h = height;
	Vector scale = Vector((float)-nx, 1.0f);
	Coin::animations_set.Get(type).at(state)->Render(p.x, p.y, 255, w, h, scale);
	p.y = p.y - (h - this->height) / 2;
	p.x = p.x - (w - this->width) /2;
	width = w;
	height = h;
	RenderBoundingBox();
}



void Coin::SetState(string state)
{
	if (state == "running") {
	}
	else if (state == "fromMisteryBox") {
		p.y = p.y - 20;
		oldP = p;
		v.y = -0.5f;
		g.y = 0.0015f;
	}
	else if (state == "hidden") {

	}

	CGameObject::SetState(state);

}

void Coin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}


void Coin::HandleCollision(LPCOLLISIONEVENT e) {
	if (isAllowCollision == true && state != "hidden") {
		SetState("hidden");
		isAllowCollision = false;
	}
}

void Coin::OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
	if (Test* player = dynamic_cast<Test*>(obj)) {
		this->SetState("hidden");
	}
}
