#pragma once
#include "GameObject.h"
#include "MapEntity.h"
#include "Game.h"
#include "Enemy.h"
#include <iostream>

class RedGoomba :public Enemy<RedGoomba>
{

public:
	RedGoomba();
	int countJump = 0;
	Vector oldP;
	virtual void SetState(string state);
	virtual void HandleCollision(LPCOLLISIONEVENT e);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Jump(float vy);
	virtual void OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event);
	virtual void BeingKill();
	virtual void CollisionVertical(LPGAMEOBJECT obj, LPCOLLISIONEVENT event);
	virtual void HandleAfterCreated();

	static json spriteData;
	static json data;
	static LPDIRECT3DTEXTURE9 texture;
	static unordered_map<string, LPSPRITE> sprites; //save all sprite of animation
	static unordered_map<string, LPANIMATION> all_animations; //save all animations
	static CAnimationSets animations_set; //save all the animation 
};