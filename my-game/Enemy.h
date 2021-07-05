#pragma once
#include "GameObject.h"
#include "Game.h"
#include <iostream>
#include "Test.h"
#include "MapEntity.h"


#include "library/json.hpp"

using json = nlohmann::json;


template <class T>
class Enemy : public MapEntity<T>
{
public:

	Enemy() {

	}

	
	bool useLimit = true;
	Vector walkingLimit = Vector(0, 0);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
		isUniversal = true;
	}

	virtual void KillPlayer(Test* obj) {
		obj->Die();
	}
	virtual void BeingKill() {
		SetState("die");
	}
	virtual void Transform() {

	}

	virtual void HandleCollision(LPCOLLISIONEVENT e) {
		if (e->ny != 0) {
			FindWalkingLimit(e->obj);
		}
		if (e->nx != 0) {
			if (e->obj->name == "RectPlatform" && e->ny == 0) {
				p.x = p.x + d.x;
			}
			else
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
};