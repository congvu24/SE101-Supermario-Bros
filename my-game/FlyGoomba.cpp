#include "FlyGoomba.h"
#include "Vector.h"
#include "Test.h"
#include "PlayScence.h"
#include "MiniGoomba.h"
#include <iostream>


#define JUMP_TIME 1000
#define FLY_TIME 5000
#define RUN_TIME 3000

LPDIRECT3DTEXTURE9 FlyGoomba::texture = NULL;
unordered_map<string, LPSPRITE> FlyGoomba::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> FlyGoomba::all_animations; //save all animations
CAnimationSets FlyGoomba::animations_set; //save all the animation sets
json FlyGoomba::data = NULL;

FlyGoomba::FlyGoomba()
{
	SetState("running");
	v = Vector(-0.05f, 0);
	isAllowCollision = true;
	isBlockPlayer = true;
	useLimit = false;
	point = 100;

}

void FlyGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (actionTime > 0) {
		actionTime = actionTime - dt;
	}

	if (actionTime <= 0 && state == "running") {
		SetState("jumping");
	}
	else if (actionTime <= 0 && state == "jumping") {
		SetState("flying");
	}
	else if (actionTime <= 0 && state == "flying") {
		SetState("running");
	}

	if (state == "flying" && countChild != 0) {
		if (actionTime <= countChild * 1000) {
			SpawMini();
			countChild--;
		}
	}

	if (state == "running") {
		v = v + g * dt;
		if (v.y > 0.35f) v.y = 0.35f;
	}

	Enemy::Update(dt, coObjects);
	CGameObject::Update(dt, coObjects);
	Enemy::CheckToChangeDirection();


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


void FlyGoomba::SetState(string state)
{
	if (state == "running") {
		actionTime = RUN_TIME;
	}
	else if (state == "jumping") {
		v.y = -0.3f;
		actionTime = JUMP_TIME;
	}
	else if (state == "flying") {
		v.y = 0;
		countChild = 5;
		actionTime = FLY_TIME;
	}
	else if (state == "die") {
		v = Vector(0, -0.15f);
	}
	else if (state == "hidden") {

	}

	CGameObject::SetState(state);

}

void FlyGoomba::HandleCollision(LPCOLLISIONEVENT e) {
	Enemy::HandleCollision(e);
}

void FlyGoomba::Die() {
}

void FlyGoomba::BeingKill() {
	Enemy::BeingKill();
	isAllowCollision = false;
}

void FlyGoomba::OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
	Enemy::OnHadCollided(obj, event);

	if (Test* player = dynamic_cast<Test*>(obj)) {
		if (isBlockPlayer == true) {
			isAllowCollision = false;
			isBlockPlayer = false;
		}
	}
}

void FlyGoomba::SpawMini() {
	LPGAMEOBJECT bullet = new MiniGoomba();
	bullet->p = p;
	CGame::GetInstance()->GetCurrentScene()->addObject(bullet);
}