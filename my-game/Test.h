#pragma once
#include "GameObject.h"
#include "Character.h"
#include "Game.h"
#include <iostream>


#define SmallMario "1"
#define BigMario "2"
#define RacconMario "3"


class Test : public Character
{
public:
	Test();
	bool canJump = true;
	bool isReadyChangeState = true;
	float max_move_x;
	float max_move_y;
	float powerX = 0;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(string state);
	virtual void HandleCollision(LPCOLLISIONEVENT e);
	virtual void HandleCollisionVertical(LPCOLLISIONEVENT e);
	virtual void HandleCollisionHorizontal(LPCOLLISIONEVENT e);
	virtual void Die();
	virtual void Transform();
	virtual void ProcessKeyboard(KeyboardEvent kEvent);
	virtual void OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {}
};