#include "Koopas.h"
#include "Vector.h"
#include "Test.h"
#include "PlayScence.h"
#include "RedGoomba.h"
#include <iostream>


LPDIRECT3DTEXTURE9 Koopas::texture = NULL;
unordered_map<string, LPSPRITE> Koopas::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> Koopas::all_animations; //save all animations
CAnimationSets Koopas::animations_set; //save all the animation sets
json Koopas::data = NULL;

Koopas::Koopas()
{
	SetState("running");
	v = Vector(0.05f, 0);
	isBlockPlayer = true;
	useLimit = true;
}

void Koopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	Enemy::CheckToChangeDirection();

	v = v + g * dt;
	if (v.y > 0.35f) v.y = 0.35f;

	if (v.x > 0) nx = 1;
	else nx = -1;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEOBJECT>* checkObjects = new vector<LPGAMEOBJECT>();

	coEvents.clear();
	for (auto i = coObjects->begin(); i != coObjects->end(); i++)
	{
		if (state == "die") {
			if ((*i)->isAllowCollision == true) {
				checkObjects->push_back((*i));
			}
		}
		else {
			if ((*i)->isAllowCollision == true) {
				checkObjects->push_back((*i));
			}
		}

	}

	coEvents.clear();
	CalcPotentialCollisions(checkObjects, coEvents);

	if (coEvents.size() == 0) {
		p = p + d;
	}
	else {
		//p.x = p.x + d.x;

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		if (ny != 0) v.y = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++) {

			HandleCollision(coEventsResult[i]);
		}

		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	coEvents.clear();
	coEventsResult.clear();
	checkObjects->clear();
	delete checkObjects;
}


void Koopas::SetState(string state)
{
	if (state == "running") {
	}
	else if (state == "die") {
		v = Vector(0, 0);
	}
	else if (state == "hidden") {

	}

	CGameObject::SetState(state);

}

void Koopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}



void Koopas::HandleCollision(LPCOLLISIONEVENT e) { // xử lí collision do chính mình detect: chạm Goomba khi đã bị hit
	Enemy::HandleCollision(e);
	if (e->obj->name == "RectCollision") {
		useLimit = false;
	}

	if (e->obj->name != "RectPlatform" && e->obj->name != "RectCollision" && !CPlayScene::IsPlayer(e->obj))
		DebugOut(L"Collision with: %s \n", ToLPCWSTR(e->obj->name));

	if (state == "die" && isHitted == true && e->nx != 0) {
		if (Goomba* obj = dynamic_cast<Goomba*>(e->obj)) {
			obj->BeingKill();
			this->v.x = 0.5f * -e->nx;
		}
		if (RedGoomba* obj = dynamic_cast<RedGoomba*>(e->obj)) {
			obj->BeingKill();
			this->v.x = 0.5f * -e->nx;
		}
		if (MisteryBox* obj = dynamic_cast<MisteryBox*>(e->obj)) {
			obj->GiveReward();
		}
	}
}


void Koopas::OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
	Enemy::OnHadCollided(obj, event);

	if (Test* player = dynamic_cast<Test*>(obj)) {
		if (state == "die") {
			if (isHitted == false && event->nx != 0) {
				player->SetState("kick");
				isUniversal = true;
				this->v.x = 0.5f * -event->nx;
				isHitted = true;
				isBlockPlayer = true;
				useLimit = false;
			}
			else if (isHitted == true && event->nx != 0) {
				KillPlayer(player);
				this->v.x = 0.5f * event->nx;
			}
		}
	}
}