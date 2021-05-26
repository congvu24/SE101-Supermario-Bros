#pragma once
#include "GameObject.h"
#include <iostream>



class Collision : public CGameObject
{

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void ParseFromJson(json data);
	virtual void Render();

public:
	Collision();
	virtual void SetState(string state);
};