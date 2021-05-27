#pragma once
#include "GameObject.h"
#include <iostream>



class Coin : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	Coin();
	virtual void SetState(string state);
};