#pragma once
#include "GameObject.h"
#include "Game.h"
#include <iostream>



class Coin : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	//virtual void SaveStaticData(json data);
	virtual void Render();

public:
	Coin();
	virtual void SetState(string state);
	void ParseFromOwnJson();

	static LPDIRECT3DTEXTURE9 texture;
	static unordered_map<string, LPSPRITE> sprites; //save all sprite of animation
	static unordered_map<string, LPANIMATION> all_animations; //save all animations
	static CAnimationSets animations_set; //save all the animation sets
	static void SaveStaticData(json data);
	static void SetTexture(LPCWSTR filePath, D3DCOLOR transparentColor);
	static void ParseSpriteFromJson(LPCWSTR filePath);
	static void ParseAnimationFromJson(LPCWSTR filePath);
	static void AddSprite(string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);
	static LPSPRITE GetSprite(string id);
	static void AddAnimation(string id, LPANIMATION ani);
	static LPANIMATION GetAnimation(string id);
	static void AddAnimationSet(string id, LPANIMATION_SET ani_set);
};