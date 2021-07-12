#pragma once
#include "GameObject.h"
#include "MapEntity.h"
#include "Game.h"
#include "Koopas.h"
#include <iostream>

class FlyKoopas :public Koopas
{

public:
	FlyKoopas();
	bool canJump = false;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(string state);
};