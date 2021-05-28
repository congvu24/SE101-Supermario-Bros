#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "GameObject.h"
#include "Sprites.h"
#include "Animations.h"
#include "library/json.hpp"
#include "Utils.h"
#include "Vector.h"

using json = nlohmann::json;

using namespace std;

class Character : public CGameObject
{

public:

	unordered_map<string, LPSPRITE> sprites; //save all sprite of animation
	unordered_map<string, LPANIMATION> all_animations; //save all animations
	CAnimationSets animations_set; //save all the animation sets

	//string active_animation_set; //active animation set to have different type of character
	LPANIMATION_SET active_animation_set;

	vector<LPCOLLISIONEVENT> coEvents;


public:
	string GetState() { return this->state; }
	void SetActiveAnimationSet(string ani_set_id) { active_animation_set = animations_set.Get(ani_set_id); }
	Character();

	virtual void Render() = 0;

	void ParseFromJson(json data); // use this function to parse from data to object
	void ParseFromOwnJson(); // use this function to parse from data to object


	void AddSprite(string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);
	LPSPRITE GetSprite(string id);
	void ClearSprite();

	void AddAnimation(string id, LPANIMATION ani);
	LPANIMATION GetAnimation(string id);
	void ClearAnimation();

	void SetTexture(LPCWSTR filePath, D3DCOLOR transparentColor);
	LPDIRECT3DTEXTURE9 GetTexture();

	void AddAnimationSet(string id, LPANIMATION_SET ani_set);
	LPANIMATION_SET GetAnimationSet(string id);
	void ClearAnimationSet();

	void ParseSpriteFromJson(LPCWSTR filePath);
	void ParseAnimationFromJson(LPCWSTR filePath);

	~Character();
};

