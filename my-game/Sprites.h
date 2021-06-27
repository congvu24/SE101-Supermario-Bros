#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include "Vector.h"
#include <unordered_map>

using namespace std;

class CSprite
{
	string id;				// Sprite ID in the sprite database

	float left;
	float top;
	float right;
	float bottom;

	LPDIRECT3DTEXTURE9 texture;
public:
	float width;
	float height;
	CSprite(string id, float left, float top, float right, float bottom, float width, float height, LPDIRECT3DTEXTURE9 tex);

	void Draw(float x, float y, int alpha = 255);
	void DrawWithScale(float x, float y, Vector scale = Vector(1.0f, 1.0f), int alpha = 255);
};

typedef CSprite* LPSPRITE;

/*
	Manage sprite database
*/
class CSprites
{
	static CSprites* __instance;

	unordered_map<string, LPSPRITE> sprites;

public:
	void Add(string id, float left, float top, float right, float bottom, LPDIRECT3DTEXTURE9 tex);
	LPSPRITE Get(string id);
	void CSprites::Clear();

	static CSprites* GetInstance();
};



