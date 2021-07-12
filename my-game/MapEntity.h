#pragma once
#include "GameObject.h"
#include "Game.h"
#include <iostream>

#include "library/json.hpp"

using json = nlohmann::json;


template <class T>
class MapEntity : public CGameObject
{
public:

	MapEntity() {
		SetState("running");
		nx = -1;
	}
	virtual void  Render()
	{
		float w = width;
		float h = height;
		Vector scale = Vector((float)-nx, 1.0f);
		T::animations_set.Get(type).at(state)->Render(p.x, p.y, 255, w, h, scale);
		if (abs(h - this->height) >= 10) {
			p.y = p.y - (h - this->height);
			height = h;
		}
		if (abs(w - this->width) >= 10) {
			p.x = p.x - (w - this->width);
			width = w;
		}
		RenderBoundingBox();
	}

	void ParseFromOwnJson() {
		if (T::data != NULL) {
			int id = stoi(string(T::data["id"])); //object id;
			string name = T::data["name"]; //object name;
			string type = to_string(T::data["type"]); //object type;
			this->id = id;
			if (this->type == "") this->type = type;
			//this->name = name;
			//SetActiveAnimationSet(type);
			HandleAfterCreated();
		}
	}
	void ParseFromJson(json data) {};
	void SetActiveAnimationSet(string ani_set_id) {
		active_animation_set = T::animations_set.Get(ani_set_id);
	}


	void SetState(string state) {
		this->state = state;
	}

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
		T::texture = CGame::LoadTexture(filePath);
	}
	static void ParseSpriteFromJson(LPCWSTR filePath) {
		OutputDebugString(filePath);
		json sprite = ReadJsonFIle(filePath);
		json frames = sprite["frames"];


		for (json::iterator it = frames.begin(); it != frames.end(); ++it) {

			json data = it.value();
			string id = it.key();
			json frame = data["frame"];

			float l = frame["x"];
			float t = frame["y"];
			float r = l + frame["w"];

			float b = t + frame["h"];

			if (T::texture == NULL)
			{
				DebugOut(L"[ERROR] Texture ID %d not found!\n");
				return;
			}

			MapEntity::AddSprite(id, l, t, r, b, T::texture);
		}
		DebugOut(L"[INFO] Get Texture success : %s\n", filePath);

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
		DebugOut(L"[INFO] Get Animation success : %s\n", filePath);
	}
	static void AddSprite(string id, float left, float top, float right, float bottom, LPDIRECT3DTEXTURE9 tex) {
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

	void clear() {
		/*T::all_animations.clear();
		T::animations_set.Clear();
		T::sprites.clear();
		T::texture = NULL;
		T::data = NULL;*/
		CGameObject::clear();
	}

	virtual void HandleAfterCreated() {
	}
};