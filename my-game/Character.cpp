#include <d3dx9.h>
#include <algorithm>


#include "Utils.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"
#include "Character.h"

Character::Character()
{
}




void Character::AddSprite(string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex) {
	LPSPRITE s = new CSprite(id, left, top, right, bottom, right - left, bottom - top, tex);
	sprites[id] = s;

}
LPSPRITE Character::GetSprite(string id) {
	LPSPRITE sprite = sprites.at(id);
	return sprite;
}
void Character::ClearSprite() {
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}


void Character::AddAnimation(string id, LPANIMATION ani) {
	all_animations[id] = ani;
}
LPANIMATION Character::GetAnimation(string id) {
	return all_animations[id];
}
void Character::ClearAnimation() {
	all_animations.clear();
}


void Character::AddAnimationSet(string id, LPANIMATION_SET ani_set) {
	animations_set.Add(id, ani_set);
}
LPANIMATION_SET Character::GetAnimationSet(string id) {
	return animations_set.Get(id);
}
void Character::ClearAnimationSet() {
	animations_set.Clear();
}

void Character::SetTexture(LPCWSTR filePath, D3DCOLOR transparentColor = D3DCOLOR_XRGB(255, 0, 255)) {

	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(filePath, &info);
	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] GetImageInfoFromFile failed: %s\n", filePath);
		return;
	}

	LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();
	LPDIRECT3DTEXTURE9 textu;

	result = D3DXCreateTextureFromFileEx(
		d3ddv,								// Pointer to Direct3D device object
		filePath,							// Path to the image to load
		info.Width,							// Texture width
		info.Height,						// Texture height
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transparentColor,		// transparentColor	
		&info,
		NULL,
		&textu);								// Created texture pointer

	if (result != D3D_OK)
	{
		OutputDebugString(L"[ERROR] CreateTextureFromFile failed\n");
		return;
	}

	texture = textu;


}

LPDIRECT3DTEXTURE9 Character::GetTexture() {
	return texture;
}



void Character::ParseSpriteFromJson(LPCWSTR filePath) {

	OutputDebugString(filePath);
	json sprite = ReadJsonFIle(filePath);
	json frames = sprite["frames"];


	for (json::iterator it = frames.begin(); it != frames.end(); ++it) {

		json data = it.value();
		string id = it.key();
		json frame = data["frame"];

		int l = frame["x"];
		int t = frame["y"];
		int r = l + frame["w"];
		//b = h - t;
		//w = r - l;

		int b = t + frame["h"];
		//int w = frame["w"];
		//int h = frame["h"];

		if (GetTexture() == NULL)
		{
			DebugOut(L"[ERROR] Texture ID %d not found!\n");
			return;
		}

		AddSprite(id, l, t, r, b, GetTexture());
	}
}
void Character::ParseAnimationFromJson(LPCWSTR filePath) {
	json data = ReadJsonFIle(filePath);

	for (json::iterator set = data.begin(); set != data.end(); ++set) {
		string key = set.key();
		json allAnimation = set.value();
		LPANIMATION_SET animation_set;

		for (json::iterator animation = allAnimation.begin(); animation != allAnimation.end(); ++animation) {
			string animationName = animation.key();
			LPANIMATION ani = new CAnimation();
			for (json::iterator frame = animation.value().begin(); frame != animation.value().end(); ++frame) {
				string name = string(frame.value()["name"]);
				LPSPRITE sprite = GetSprite(name);

				int time = frame.value()["time"];
				ani->Add(sprite, time);

			}
			AddAnimation(animationName, ani);
			animation_set[animationName] = GetAnimation(animationName);

		}
		AddAnimationSet(key, animation_set);

	}
	// load all animation set to animation_set;

}

void Character::ParseFromJson(json data) {

	int id = stoi(string(data["id"])); //object id;


	string type = to_string(data["type"]); //object type;

	float x = data["x"]; //object x;
	float y = data["y"]; //object y;

	LPCWSTR sprite = ToLPCWSTR(string(data["sprite"]));
	LPCWSTR texture = ToLPCWSTR(string(data["texture"]));
	LPCWSTR animation = ToLPCWSTR(string(data["animation"]));

	// set inital position
	this->id = id;
	this->type = type;
	this->p.x = x;
	this->p.y = y;
	//
	D3DCOLOR transcolor;
	SetTexture(texture, D3DCOLOR_XRGB(255, 0, 255));
	ParseSpriteFromJson(sprite);

	ParseAnimationFromJson(animation);

	// set active animation set;
	SetActiveAnimationSet(type);
}


void Character::ParseFromOwnJson() {}

Character::~Character()
{
	ClearSprite();
	ClearAnimation();
}