#pragma once
#include "GameObject.h"
#include "MapEntity.h"
#include "Game.h"
#include "BoomerangBrother.h"
#include <iostream>
#include "Enemy.h"

#define FLY_TIME 2000
#define ANCHOR_DISTANCE_X 200
#define ANCHOR_DISTANCE_Y 100


enum class BoomerangDirection {
	Forward,
	Goback
};

class Boomerang :public Enemy<Boomerang>
{
public:
	Boomerang();
	BoomerangBrother* parent;
	BoomerangDirection moveDirection = BoomerangDirection::Forward;
	float count = 0.0f;
	Vector direction;
	Vector distance;
	Vector destination;
	float angle;
	Vector oldP;
	int flyTime = FLY_TIME;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void HandleCollision(LPCOLLISIONEVENT e);
	virtual void SetState(string state);
	virtual void OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event);
	virtual void BeingKill() {};
	virtual void GiveReward() {};

	static json spriteData;
	static json data;
	static LPDIRECT3DTEXTURE9 texture;
	static unordered_map<string, LPSPRITE> sprites; //save all sprite of animation
	static unordered_map<string, LPANIMATION> all_animations; //save all animations
	static CAnimationSets animations_set; //save all the animation 
};