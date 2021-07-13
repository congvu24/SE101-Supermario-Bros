#include "Goomba.h"
#include "Vector.h"
#include "Test.h"
#include "PlayScence.h"
#include <iostream>


LPDIRECT3DTEXTURE9 Goomba::texture = NULL;
unordered_map<string, LPSPRITE> Goomba::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> Goomba::all_animations; //save all animations
CAnimationSets Goomba::animations_set; //save all the animation sets
json Goomba::data = NULL;

Goomba::Goomba()
{
	SetState("running");
	v = Vector(-0.05f, 0);
	isAllowCollision = true;
	isBlockPlayer = true;
	useLimit = false;
}

void Goomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Enemy::Update(dt, coObjects);
	CGameObject::Update(dt, coObjects);
	Enemy::CheckToChangeDirection();

	v = v + g * dt;
	if (v.y > MAX_VY) v.y = MAX_VY;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	vector<LPGAMEOBJECT>* checkObjects = new vector<LPGAMEOBJECT>();

	coEvents.clear();


	for (auto i = coObjects->begin(); i != coObjects->end(); i++)
	{
		if ((*i)->isAllowCollision == true && !CPlayScene::IsPlayer(*i)) {
			checkObjects->push_back((*i));
		}
	}

	CalcPotentialCollisions(checkObjects, coEvents);


	if (coEvents.size() == 0) {

		p = p + d;
	}
	else {
		p.x = p.x + d.x;

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


		/*	if (nx != 0) v.x = -v.x;*/
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


void Goomba::SetState(string state)
{
	if (state == "running") {
	}
	else if (state == "die") {
		v = Vector(0, -0.15f);
	}
	else if (state == "hidden") {

	}

	CGameObject::SetState(state);

}

void Goomba::HandleCollision(LPCOLLISIONEVENT e) {
	Enemy::HandleCollision(e);
}

void Goomba::Die() {
}

void Goomba::BeingKill() {
	Enemy::BeingKill();
	isAllowCollision = false;
	isBlockPlayer = false;
}

void Goomba::OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
	Enemy::OnHadCollided(obj, event);
}