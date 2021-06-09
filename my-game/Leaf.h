#pragma once
#include "GameObject.h"
#include "MapEntity.h"
#include "Game.h"
#include <iostream>


#define LEAF_SPEED	    0.003f
#define LEAF_AMPLITUDE	60 
#define PI 3.1415926535897932384626433832795028841971693993751058209 


class Leaf :public MapEntity<Leaf>
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	Vector oldP;
	//virtual void  Render();
	int beginFalling = 0;

public:
	Leaf();

	virtual void SetState(string state);
	virtual void HandleCollision(LPCOLLISIONEVENT e);


	static json data;
	static LPDIRECT3DTEXTURE9 texture;
	static unordered_map<string, LPSPRITE> sprites; //save all sprite of animation
	static unordered_map<string, LPANIMATION> all_animations; //save all animations
	static CAnimationSets animations_set; //save all the animation 
};