#pragma once
#include "GameObject.h"
#include "MapEntity.h"
#include "Game.h"
#include <iostream>
#include "Enemy.h"

class VenusBullet :public Enemy<VenusBullet>
{
	
	//virtual void  Render();
public:
	VenusBullet();

	Vector direction;
	Vector distance;
	float angle;
	Vector oldP;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void HandleCollision(LPCOLLISIONEVENT e);
	virtual void SetState(string state);
	virtual void OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event);
	virtual void BeingKill() {};

	static json data;
	static LPDIRECT3DTEXTURE9 texture;
	static unordered_map<string, LPSPRITE> sprites; //save all sprite of animation
	static unordered_map<string, LPANIMATION> all_animations; //save all animations
	static CAnimationSets animations_set; //save all the animation 
};