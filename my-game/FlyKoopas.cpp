#include "Koopas.h"
#include "Vector.h"
#include "Test.h"
#include "PlayScence.h"
#include <iostream>
#include "FlyKoopas.h"


FlyKoopas::FlyKoopas()
{
	SetState("running");
	type = "fly";
	v = Vector(-0.05f, 0);
	isAllowCollision = true;
	isBlockPlayer = true;
	useLimit = false;
}

void FlyKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Enemy::Update(dt, coObjects);

	v = v + g * dt;
	if (v.y > 0.35f) v.y = 0.35f;

	CGameObject::Update(dt, coObjects);

	if (canJump == true && state != "die") {
		SetState("running");
	}

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
			if (ny < 0) canJump = true;
			HandleCollision(coEventsResult[i]);
		}

		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	coEvents.clear();
	coEventsResult.clear();
	checkObjects->clear();
	delete checkObjects;
}


void FlyKoopas::SetState(string state) {
	if (state == "running" && canJump == true) {
		v.y = -0.3;
		canJump = false;
		CGameObject::SetState(state);
	}
	else {
		Koopas::SetState(state);
	}
}