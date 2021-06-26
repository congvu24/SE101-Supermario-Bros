#pragma once
#include "GameObject.h"
#include "MapEntity.h"
#include "Game.h"
#include <iostream>


#define Mushroom_SPEED	    0.003f
#define Mushroom_AMPLITUDE	60 
#define PI 3.1415926535897932384626433832795028841971693993751058209 


class Mushroom :public MapEntity<Mushroom>
{
	

public:
	Mushroom();

	Vector oldP;
	int beginFalling = 0;

	virtual void SetState(string state);
	virtual void HandleCollision(LPCOLLISIONEVENT e);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	static json data;
	static LPDIRECT3DTEXTURE9 texture;
	static unordered_map<string, LPSPRITE> sprites; //save all sprite of animation
	static unordered_map<string, LPANIMATION> all_animations; //save all animations
	static CAnimationSets animations_set; //save all the animation 
};