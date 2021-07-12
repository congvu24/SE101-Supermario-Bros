#pragma once
#include "GameObject.h"
#include "MapEntity.h"
#include "Game.h"
#include "Enemy.h"

#define TIME_WAITING 5000

enum class BoomerangBrotherAction {
	ATTACK,
	MOVING,
	THROW
};

class BoomerangBrother :public Enemy<BoomerangBrother>
{

public:
	BoomerangBrother();
	BoomerangBrotherAction action;

	Vector oldP;
	int timeWaiting = TIME_WAITING;
	virtual void SetState(string state);
	virtual void SetAction(BoomerangBrotherAction newAction);
	virtual void HandleCollision(LPCOLLISIONEVENT e);
	virtual void HandleAfterCreated();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void BeingKill();
	virtual void OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event);


	static json data;
	static LPDIRECT3DTEXTURE9 texture;
	static unordered_map<string, LPSPRITE> sprites; //save all sprite of animation
	static unordered_map<string, LPANIMATION> all_animations; //save all animations
	static CAnimationSets animations_set; //save all the animation 
};