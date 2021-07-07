#pragma once
#include "GameObject.h"
#include "MapEntity.h"
#include "Game.h"
#include "Coin.h"
#include "Leaf.h"
#include <iostream>


template <class T>
class Box :public MapEntity<T>
{

public:
	Vector oldP;
	LPGAMEOBJECT hitObject;
	int jumpDirection = 1;
	bool isHitted = false;
	bool isJumping = false;
	bool allowToHitBottom = true;
	bool allowToHitTop = false;
	bool allowToHitRight = false;
	bool allowToHitLeft = false;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
		v = v + g * dt;
		if (v.y > 0.35f) v.y = 0.35f;

		CGameObject::Update(dt, coObjects);

		Vector newPos = p + d;
		if ((isJumping == true && newPos.y >= oldP.y && jumpDirection == -1) || (isJumping == true && newPos.y <= oldP.y && jumpDirection == 1)) {
			p = oldP;
			isJumping = false;
			AfterJump();
		}
		else {
			p = p + d;
		}
	}
	virtual void GiveReward() {}
	virtual void OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
		if (event->ny != 0) {
			OnHadCollidedVertical(obj, event);
		}
		if (event->nx != 0) {
			OnHadCollidedHorizontal(obj, event);
		}
	}
	virtual void OnHadCollidedHorizontal(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {}
	virtual void OnHadCollidedVertical(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
		if (allowToHitBottom && event->ny > 0) {
			JumpUp(obj);
		}
		if (allowToHitTop && event->ny < 0) {
			JumpDown(obj);
		}
	}
	virtual void JumpUp(LPGAMEOBJECT obj) {
		if (isHitted == false && !isJumping) {
			isJumping = true;
			//isHitted = true;
			oldP = p;
			v.y = -0.3f;
			jumpDirection = -1;
			g.y = 0.001f;
			//obj->g.y = -0.001f;
		}
	}
	virtual void JumpDown(LPGAMEOBJECT obj) {
		if (isHitted == false && !isJumping) {
			isJumping = true;
			//isHitted = true;
			oldP = p;
			v.y = 0.2f;
			jumpDirection = 1;
			g.y = -0.001f;
			hitObject = obj;
			hitObject->g.y = -0.001f;
			hitObject->v.y = 0.2f;
		}
	}
	virtual void AfterJump() {
		if (hitObject != NULL) {
			hitObject->g.y = 0.001f;
		}
		g.y = 0;
		v.y = 0;
	}

};