#include "BoomerangBrother.h"
#include "Vector.h"
#include "Boomerang.h"
#include "PlayScence.h"
#include <iostream>

#define TIME_ATTACK 3000
#define ATTACK_DISTANCE 400

LPDIRECT3DTEXTURE9 BoomerangBrother::texture = NULL;
unordered_map<string, LPSPRITE> BoomerangBrother::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> BoomerangBrother::all_animations; //save all animations
CAnimationSets BoomerangBrother::animations_set; //save all the animation sets
json BoomerangBrother::spriteData = NULL;
json BoomerangBrother::data = NULL;


BoomerangBrother::BoomerangBrother()
{
	SetState("running");
	v = Vector(-0.015f, 0);
	action = BoomerangBrotherAction::MOVING;
	renderOrder = 1;
	isBlockPlayer = true;
}

void BoomerangBrother::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (action == BoomerangBrotherAction::THROW) return;
	CGameObject::Update(dt, coObjects);

	Vector playerPos = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->player->p;
	Vector bulletBeginPos = Vector(p.x, p.y);
	Vector distance = playerPos - bulletBeginPos;

	if (playerPos.x >= bulletBeginPos.x) v.x = 0.015f;
	else v.x = -0.015f;

	if (v.x > 0) nx = 1;
	else nx = -1;


	if (timeWaiting >= 0 && timeWaiting - dt >= 0) {
		timeWaiting = timeWaiting - dt;
	}


	if (timeWaiting <= 0) {
		SetAction(BoomerangBrotherAction::ATTACK);
	}

	if (action == BoomerangBrotherAction::ATTACK && abs(distance.x) <= ATTACK_DISTANCE && state != "die") {
		LPGAMEOBJECT bullet = new Boomerang();
		Vector direction = Vector::Normalize(distance);
		bullet->ParseFromOwnJson();
		bullet->p = bulletBeginPos;
		bullet->nx = bulletBeginPos.x >= playerPos.x ? -1 : 1;
		((Boomerang*)bullet)->oldP = bulletBeginPos;
		((Boomerang*)bullet)->destination = playerPos;
		((Boomerang*)bullet)->direction = direction;
		((Boomerang*)bullet)->parent = this;
		CGame::GetInstance()->GetCurrentScene()->addObject(bullet);
		timeWaiting = TIME_ATTACK;
		SetAction(BoomerangBrotherAction::THROW);
	}

	Enemy::CheckToChangeDirection(); 

	v = v + g * (float)dt;
	if (v.y > MAX_VY) v.y = MAX_VY;
	if (v.x >= 0) nx = 1; else nx = -1;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	vector<LPGAMEOBJECT>* checkObjects = new vector<LPGAMEOBJECT>();

	coEvents.clear();


	for (auto i = coObjects->begin(); i != coObjects->end(); i++)
	{
		if ((*i)->isAllowCollision == true && !CPlayScene::IsPlayer(*i) && state != "die") {
			checkObjects->push_back((*i));
		}
	}

	CalcPotentialCollisions(checkObjects, coEvents);

	if (state == "die") {
		p = p + d;
	}

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

void BoomerangBrother::Render() {
	Enemy::Render();
}

void BoomerangBrother::SetState(string state)
{
	CGameObject::SetState(state);
}

void BoomerangBrother::SetAction(BoomerangBrotherAction newAction) {
	if (newAction == BoomerangBrotherAction::MOVING) {
		SetState("running");
	}
	else if (newAction == BoomerangBrotherAction::ATTACK) {
		SetState("attack");
	}
	else if (newAction == BoomerangBrotherAction::THROW) {
		SetState("running");
	}

	this->action = newAction;
}

void BoomerangBrother::HandleCollision(LPCOLLISIONEVENT e) {
	Enemy::HandleCollision(e);

	if (Boomerang* boomerang = dynamic_cast<Boomerang*>(e->obj)) {
		SetAction(BoomerangBrotherAction::MOVING);
	}
}

void BoomerangBrother::HandleAfterCreated() {
	oldP = p;
}

void BoomerangBrother::OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
	Enemy::OnHadCollided(obj, event);

	if (Boomerang* boomerang = dynamic_cast<Boomerang*>(obj)) {
		SetAction(BoomerangBrotherAction::MOVING);
	}
	
	if (obj->name == "Death") {
		SetState("hidden");
	}
}

void BoomerangBrother::BeingKill() {
	if (state != "die") {
		SetState("die");
		isAllowCollision = false;
		isBlockPlayer = false;
	}
}