//#pragma once
//#include "GameObject.h"
//#include "Game.h"
//#include "MapEntity.h"
//#include "PlayScence.h"
//
//
//template <class T>
//class Enemy : public MapEntity<T>
//{
//public:
//	virtual void KillPlayer(Test* obj) {
//		obj->Die();
//	}
//	virtual void BeingKill() {
//		SetState("die");
//	}
//	virtual void Transform() {
//
//	}
//
//	virtual void CollisionHorizontal(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
//		if (Test* player = dynamic_cast<Test*>(obj)) {
//			if (state != "die") {
//				if (event->nx != 0 && event->ny == 0) {
//					KillPlayer(player);
//				}
//			}
//		}
//	}
//	virtual void CollisionVertical(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
//		if (Test* player = dynamic_cast<Test*>(obj)) {
//			if (state != "die") {
//				if (event->ny < 0) {
//					BeingKill();
//				}
//			}
//		}
//	}
//
//	virtual void OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
//
//		if (event->nx != 0) {
//			CollisionHorizontal(obj, event);
//		}
//
//		if (event->ny != 0) {
//			CollisionVertical(obj, event);
//		}
//	}
//};


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
	virtual void KillPlayer(Test* obj) {
		obj->Die();
	}
	virtual void BeingKill() {
		SetState("die");
	}
	virtual void Transform() {

	}

	virtual void CollisionHorizontal(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
		if (Test* player = dynamic_cast<Test*>(obj)) {
			if (state != "die") {
				if (event->nx != 0 && event->ny == 0) {
					KillPlayer(player);
				}
			}
		}
	}
	virtual void CollisionVertical(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
		if (Test* player = dynamic_cast<Test*>(obj)) {
			if (state != "die") {
				if (event->ny < 0) {
					BeingKill();
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
};