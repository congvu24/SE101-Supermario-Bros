#include <d3dx9.h>
#include <algorithm>


#include "Utils.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;


	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(L"assets/texture/bbox.png", &info);
	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] GetImageInfoFromFile failed: %s\n", L"assets/texture/bbox.png");
		return;
	}

	LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();
	LPDIRECT3DTEXTURE9 textu;

	result = D3DXCreateTextureFromFileEx(
		d3ddv,								// Pointer to Direct3D device object
		L"assets/texture/bbox.png",							// Path to the image to load
		info.Width,							// Texture width
		info.Height,						// Texture height
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(255, 0, 255),		// transparentColor	
		&info,
		NULL,
		&textu);								// Created texture pointer

	if (result != D3D_OK)
	{
		OutputDebugString(L"[ERROR] CreateTextureFromFile failed\n");
		return;
	}

	bboxtex = textu;

}

void CGameObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CGameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = this->dx - sdx;
	float rdy = this->dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	CGame::SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, rdx, rdy, coO);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CGameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CGameObject::FilterCollision(
	vector<LPCOLLISIONEVENT>& coEvents,
	vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny, float& rdx, float& rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}


void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	if (bboxtex != NULL)
		CGame::GetInstance()->Draw(x, y, bboxtex, rect.left, rect.top, rect.right, rect.bottom, 100);
}




void CGameObject::AddSprite(string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex) {
	LPSPRITE s = new CSprite(id, left, top, right, bottom, right - left, bottom - top, tex);
	sprites[id] = s;

	DebugOut(L"[INFO] sprite added: %s \n", IntToLPCWSTR(right));
}
LPSPRITE CGameObject::GetSprite(string id) {
	for (auto& x : sprites) {
		DebugOut(L"[INFO] sprite added: %s \n", ToLPCWSTR(x.first));

	}
	LPSPRITE sprite = sprites.at(id);
	return sprite;
}
void CGameObject::ClearSprite() {
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}


void CGameObject::AddAnimation(string id, LPANIMATION ani) {
	all_animations[id] = ani;
}
LPANIMATION CGameObject::GetAnimation(string id) {
	return all_animations[id];
}
void CGameObject::ClearAnimation() {
	all_animations.clear();
}


void CGameObject::AddAnimationSet(string id, LPANIMATION_SET ani_set) {
	animations_set.Add(id, ani_set);
}
LPANIMATION_SET CGameObject::GetAnimationSet(string id) {
	return animations_set.Get(id);
}
void CGameObject::ClearAnimationSet() {
	animations_set.Clear();
}

void CGameObject::SetTexture(LPCWSTR filePath, D3DCOLOR transparentColor = D3DCOLOR_XRGB(255, 0, 255)) {

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

LPDIRECT3DTEXTURE9 CGameObject::GetTexture() {
	return texture;
}



void CGameObject::ParseSpriteFromJson(LPCWSTR filePath) {

	OutputDebugString(filePath);
	json sprite = ReadJsonFIle(filePath);
	json frames = sprite["frames"];


	for (json::iterator it = frames.begin(); it != frames.end(); ++it) {

		json data = it.value();
		string id = it.key();
		json frame = data["frame"];
		DebugOut(L"[INFOR] load ! %s\n", ToLPCWSTR(id));

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
void CGameObject::ParseAnimationFromJson(LPCWSTR filePath) {
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
				DebugOut(L"[INFOR] Add frame ! %s\n", ToLPCWSTR(frame.value()["name"].dump()));

			}
			AddAnimation(animationName, ani);
			animation_set[animationName] = GetAnimation(animationName);
			DebugOut(L"[INFOR] Add animation ! %s\n", ToLPCWSTR(animationName));

		}
		AddAnimationSet(key, animation_set);
		DebugOut(L"[INFOR] Add animation set ! %s\n", ToLPCWSTR(key));

	}
	// load all animation set to animation_set;

}


CGameObject::~CGameObject()
{
	ClearSprite();
	ClearAnimation();
}