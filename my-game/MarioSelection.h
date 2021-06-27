#pragma once
#include "GameObject.h"
#include "Character.h"
#include <iostream>



class MarioSelection : public Character
{


public:
	MarioSelection();
	string up;
	string right;
	string down;
	string left;
	bool isMoving = false;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(string state);
	virtual void HandleCollision(LPCOLLISIONEVENT e);
	virtual void ProcessKeyboard(KeyboardEvent kEvent);
	virtual void OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {}

};