#pragma once
#include "GameObject.h"
#include "Game.h"
#include <iostream>

#include "library/json.hpp"

using json = nlohmann::json;


template <class T>
class MapEntity : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = p.x;
		top = p.y;
		right = p.x + width;
		bottom = p.y + height;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
	{
		CGameObject::Update(dt, coObjects);

	}
	//virtual void SaveStaticData(json data);
	virtual void  Render()
	{
		float w = width;
		float h = height;
		T::animations_set.Get(type).at(state)->Render(p.x, p.y, 255, w, h);
		this->width = w;
		this->height = h;
		RenderBoundingBox();
	}

public:

	MapEntity() {
		SetState("running");
		width = 14;
		height = 16;
		p = Vector(0, 0);
	}

	void ParseFromOwnJson() {
		if (T::data != NULL) {
			int id = stoi(string(T::data["id"])); //object id;
			string name = T::data["name"]; //object name;
			string type = to_string(T::data["type"]); //object type;
			this->id = id;
			if (this->type == "") this->type = type;
			this->name = name;
			SetActiveAnimationSet(type);
		}
	}
	void ParseFromJson(json data) {};
	void SetActiveAnimationSet(string ani_set_id) {
		active_animation_set = T::animations_set.Get(ani_set_id);
	}


	void SetState(string state) {
		this->state = state;
	}
	virtual void HandleCollision(LPCOLLISIONEVENT e) = 0;
	// virtual function help to handle collision of mario to it

	static void SaveStaticData(json data) {
		if (T::data == NULL) {
			T::data = data;
			LPCWSTR sprite = ToLPCWSTR(string(data["sprite"]));
			LPCWSTR texture = ToLPCWSTR(string(data["texture"]));
			LPCWSTR animation = ToLPCWSTR(string(data["animation"]));

			MapEntity::SetTexture(texture, D3DCOLOR_XRGB(255, 0, 255));
			MapEntity::ParseSpriteFromJson(sprite);
			MapEntity::ParseAnimationFromJson(animation);
			DebugOut(L"[INFO] Save static data complete!\n");

		}

	}
	static void SetTexture(LPCWSTR filePath, D3DCOLOR transparentColor = D3DCOLOR_XRGB(255, 0, 255)) {

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

		T::texture = textu;
	}
	static void ParseSpriteFromJson(LPCWSTR filePath) {
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

			if (T::texture == NULL)
			{
				DebugOut(L"[ERROR] Texture ID %d not found!\n");
				return;
			}

			MapEntity::AddSprite(id, l, t, r, b, T::texture);
		}
		DebugOut(L"[INFOR] Get Texture success : %s\n", filePath);

	}
	static void ParseAnimationFromJson(LPCWSTR filePath) {
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
				MapEntity::AddAnimation(animationName, ani);
				animation_set[animationName] = ani;

			}
			MapEntity::AddAnimationSet(key, animation_set);
		}
		DebugOut(L"[INFOR] Get Animation success : %s\n", filePath);
	}
	static void AddSprite(string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex) {
		LPSPRITE s = new CSprite(id, left, top, right, bottom, right - left, bottom - top, tex);
		T::sprites[id] = s;
	}
	static LPSPRITE GetSprite(string id) {
		LPSPRITE sprite = T::sprites.at(id);
		return sprite;
	}
	static void AddAnimation(string id, LPANIMATION ani) {
		T::all_animations[id] = ani;
	}
	static LPANIMATION GetAnimation(string id) {
		return T::all_animations[id];
	}
	static void AddAnimationSet(string id, LPANIMATION_SET ani_set) {
		T::animations_set.Add(id, ani_set);
	}
};