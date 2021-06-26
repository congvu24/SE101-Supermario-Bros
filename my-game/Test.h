#pragma once
#include "GameObject.h"
#include "Character.h"
#include "Game.h"
#include <iostream>



class Test : public Character
{
public:
	Test();
	bool canJump = true;
	bool isReadyChangeState = true;
	float max_move_x;
	float max_move_y;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(string state);
	virtual void HandleCollision(LPCOLLISIONEVENT e);
	virtual void Die();
	virtual void Transform();
	virtual void ProcessKeyboard(KeyboardEvent kEvent);
};