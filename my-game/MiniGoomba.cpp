#include "MiniGoomba.h"
#include "Vector.h"
#include "Test.h"
#include <iostream>


LPDIRECT3DTEXTURE9 MiniGoomba::texture = NULL;
unordered_map<string, LPSPRITE> MiniGoomba::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> MiniGoomba::all_animations; //save all animations
CAnimationSets MiniGoomba::animations_set; //save all the animation sets
json MiniGoomba::data = NULL;

MiniGoomba::MiniGoomba()
{
	SetState("running");
	isBlockPlayer = false;
	isAllowCollision = false;
	point = 0;
	v = Vector(0.0f, 0.10f);
	type = "mini";
}

void MiniGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	p.y += v.y * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->camera;
	if (camera->isInCamPosition(this, 0) == false) {
		SetState("hidden");
	}

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
		if ((*i)->name == "Death") {
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

	for (UINT i = 0; i < coEventsResult.size(); i++) {

		if (Test* obj = dynamic_cast<Test*>(coEventsResult[i]->obj)) {
			obj->Die();
		}
		if (coEventsResult[i]->obj->name == "Death") {
			SetState("hidden");
		}

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	coEventsResult.clear();
	coEvents.clear();
}

void MiniGoomba::SetState(string state)
{
	CGameObject::SetState(state);
}

void MiniGoomba::HandleCollision(LPCOLLISIONEVENT e) {
	if (Test* player = dynamic_cast<Test*>(e->obj)) {
		KillPlayer(player);
	}
}

void MiniGoomba::OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
	if (Test* player = dynamic_cast<Test*>(event->obj)) {
		KillPlayer(player);
		SetState("hidden");
	}
}
