#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

#include "Sprites.h"

/*
Sprite animation
*/
class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;

public:
	CAnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite; this->time = time; }
	DWORD GetTime() { return time; }
	LPSPRITE GetSprite() { return sprite; }
};

typedef CAnimationFrame* LPANIMATION_FRAME;

class CAnimation
{
	DWORD lastFrameTime;
	int currentFrame;
	int defaultTime;
	vector<LPANIMATION_FRAME> frames;
public:
	CAnimation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(LPSPRITE sprite, DWORD time = 0);

	void Render(float x, float y, int alpha, float& width, float& height);
};

typedef CAnimation* LPANIMATION;

class CAnimations  //save animation list by id
{
	unordered_map<string, LPANIMATION> animations;

public:
	CAnimations();
	void Add(string id, LPANIMATION ani);
	LPANIMATION Get(string id);
	void Clear();
};

typedef unordered_map<string, LPANIMATION> LPANIMATION_SET; //list of CAnimations is CAnimationSet

//typedef CAnimationSet* LPANIMATION_SET;

/*
	Manage animation set database
*/
class CAnimationSets
{
	unordered_map<string, LPANIMATION_SET> animation_sets; //list of CAnimationSet by id is CAnimationsets

public:
	CAnimationSets();
	void Add(string id, LPANIMATION_SET ani);
	LPANIMATION_SET Get(string id);
	void Clear();
};