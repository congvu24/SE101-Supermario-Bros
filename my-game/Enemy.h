#pragma once
#include "GameObject.h"
#include "Game.h"
#include <iostream>
#include "Test.h"
#include "MapEntity.h"
#include "Effect.h"
#include "library/json.hpp"

using json = nlohmann::json;


#define MAX_VY  0.35f
#define DEFAULT_POINT  100

template <class T>
class Enemy : public MapEntity<T>
{
public:
	int point = DEFAULT_POINT;

	Enemy() {}

	bool useLimit = true;
	Vector walkingLimit = Vector(0, 0);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
		isUniversal = true;
	}

	virtual void KillPlayer(Test* obj) {
		obj->Die();
	}
	virtual void BeingKill() {
		if (state != "die") {
			SetState("die");
			GiveReward();
			isAllowCollision = false;
		}
	}
	virtual void Transform() {

	}

	virtual void HandleCollision(LPCOLLISIONEVENT e) {
		if (e->obj->name == "Death") {
			SetState("hidden");
			return;
		}

		if (e->ny != 0) {
			FindWalkingLimit(e->obj);
		}
		if (e->nx != 0) {
			if (ny == 0)
				ChangeDirection();
		}
	}

	virtual void CollisionHorizontal(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
		if (Test* player = dynamic_cast<Test*>(obj)) {

			if (state != "die" && player->action != MarioAction::ATTACK) {
				if (event->nx != 0 && event->ny == 0) {
					KillPlayer(player);
				}
			}
			else if (state != "die" && player->action == MarioAction::ATTACK) {
				if (event->nx != 0 && event->ny == 0) {
					BeingKill();
				}
			}
		}
	}
	virtual void CollisionVertical(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
		if (Test* player = dynamic_cast<Test*>(obj)) {
			if (state != "die") {
				if (event->ny < 0) {
					BeingKill();
					player->SetAction(MarioAction::JUMP);
				}
			}
		}
	}

	virtual void OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {

		if (event->nx != 0) {
			CollisionHorizontal(obj, event);
		}

		if (event->ny != 0) {
			CollisionVertical(obj, event);
		}
	}

	virtual void FindWalkingLimit(LPGAMEOBJECT obj) {
		if (useLimit == true) {
			if (walkingLimit.x == 0 && walkingLimit.y == 0) {
				float left, top, right, bottom;
				obj->GetBoundingBox(left, top, right, bottom);
				walkingLimit.x = left;
				walkingLimit.y = right;
			}
		}
	}
	virtual void ChangeDirection() {
		d.x *= -1;
		nx *= -1;
		v.x *= -1;
	}

	virtual void CheckToChangeDirection() {
		if (useLimit == true) {
			if ((p.x + d.x < walkingLimit.x || p.x + d.x + width> walkingLimit.y) && (walkingLimit.x != 0 || walkingLimit.y != 0)) {
				ChangeDirection();
			}
		}
	}

	virtual void GiveReward() {
		Effect* pointEffect = new Effect(to_string(point), 300);
		pointEffect->v.y = -0.05f;
		pointEffect->p = p;
		CGame::GetInstance()->GetCurrentScene()->addObject(pointEffect);
		CGame::GetInstance()->GetCurrentScene()->AddPoint(point);
	}
};