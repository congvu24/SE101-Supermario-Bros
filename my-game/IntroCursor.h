#pragma once
#include "GameObject.h"
#include "Character.h"
#include <iostream>



class IntroCursor : public Character
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();


public:
	IntroCursor();
	virtual void SetState(string state);
	virtual void HandleCollision(LPCOLLISIONEVENT e);
};