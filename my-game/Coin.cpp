#include "Coin.h"
#include <iostream>


LPDIRECT3DTEXTURE9 Coin::texture = NULL;
unordered_map<string, LPSPRITE> Coin::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> Coin::all_animations; //save all animations
CAnimationSets Coin::animations_set; //save all the animation sets


Coin::Coin()
{
	SetState("running");
	width = 14;
	height = 16;
}

void Coin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//CGameObject::Update(dt, coObjects);

}

void Coin::Render()
{
	float width = 0;
	float height = 0;
	Coin::animations_set.Get(type).at(state)->Render(p.x, p.y, 255, width, height);
	RenderBoundingBox();
}

void Coin::SetState(string state)
{
	CGameObject::SetState(state);

}

void Coin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}

void Coin::SaveStaticData(json data) {
	if (Coin::data == NULL) {
		Coin::data = data;
		LPCWSTR sprite = ToLPCWSTR(string(data["sprite"]));
		LPCWSTR texture = ToLPCWSTR(string(data["texture"]));
		LPCWSTR animation = ToLPCWSTR(string(data["animation"]));

		Coin::SetTexture(texture, D3DCOLOR_XRGB(255, 0, 255));
		Coin::ParseSpriteFromJson(sprite);
		Coin::ParseAnimationFromJson(animation);
	}

}
void Coin::SetTexture(LPCWSTR filePath, D3DCOLOR transparentColor = D3DCOLOR_XRGB(255, 0, 255)) {

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

	Coin::texture = textu;
}

void Coin::ParseSpriteFromJson(LPCWSTR filePath) {

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

		if (Coin::texture == NULL)
		{
			DebugOut(L"[ERROR] Texture ID %d not found!\n");
			return;
		}

		Coin::AddSprite(id, l, t, r, b, Coin::texture);
	}
	DebugOut(L"[INFOR] Get Texture success : %s\n", filePath);

}
void Coin::ParseAnimationFromJson(LPCWSTR filePath) {
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
			Coin::AddAnimation(animationName, ani);
			animation_set[animationName] = Coin::GetAnimation(animationName);

		}
		Coin::AddAnimationSet(key, animation_set);
	}
	DebugOut(L"[INFOR] Get Animation success : %s\n", filePath);

}

void Coin::AddSprite(string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex) {
	LPSPRITE s = new CSprite(id, left, top, right, bottom, right - left, bottom - top, tex);
	Coin::sprites[id] = s;
}

LPSPRITE Coin::GetSprite(string id) {
	LPSPRITE sprite = Coin::sprites.at(id);
	return sprite;
}

void Coin::AddAnimation(string id, LPANIMATION ani) {
	Coin::all_animations[id] = ani;
}
LPANIMATION Coin::GetAnimation(string id) {
	return Coin::all_animations[id];
}

void Coin::AddAnimationSet(string id, LPANIMATION_SET ani_set) {
	animations_set.Add(id, ani_set);
}

void Coin::ParseFromOwnJson() {
	if (Coin::data != NULL) {
		int id = stoi(string(Coin::data["id"])); //object id;
		string type = to_string(Coin::data["type"]); //object type;
		this->id = id;
		this->type = type;
		SetActiveAnimationSet(type);
	}

}