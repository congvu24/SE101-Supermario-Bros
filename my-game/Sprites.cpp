#include "Sprites.h"
#include "Game.h"
#include "Utils.h"

CSprite::CSprite(string id, float left, float top, float right, float bottom, float width, float height, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->width = width;
	this->height = height;
	this->texture = tex;
}

CSprites* CSprites::__instance = NULL;

CSprites* CSprites::GetInstance()
{
	if (__instance == NULL) __instance = new CSprites();
	return __instance;
}

void CSprite::Draw(float x, float y, int alpha)
{
	CGame* game = CGame::GetInstance();
	game->Draw(x, y, texture, left, top, right, bottom, alpha);
}

void CSprite::DrawWithScale(float x, float y, Vector scale, int alpha)
{
	CGame* game = CGame::GetInstance();
	Vector p = Vector(x, y);
	Rect r = { left ,top,right ,bottom };
	D3DXVECTOR2 scal = D3DXVECTOR2(scale.x, scale.y);

	game->DrawWithScale(p, texture, r, alpha, scal);
}

void CSprite::DrawPositionInCamera(float x, float y, Vector scale, int alpha)
{
	CGame* game = CGame::GetInstance();
	Vector p = Vector(x, y);
	Rect r = { left ,top,right ,bottom };
	D3DXVECTOR2 scal = D3DXVECTOR2(scale.x, scale.y);

	game->DrawPositionInCamera(p, texture, r, alpha, scal);
}


void CSprites::Add(string id, float left, float top, float right, float bottom, LPDIRECT3DTEXTURE9 tex)
{
	LPSPRITE s = new CSprite(id, left, top, right, bottom, right - left, bottom - top, tex);
	sprites[id] = s;
}

LPSPRITE CSprites::Get(string id)
{
	return sprites[id];
}

/*
	Clear all loaded textures
*/
void CSprites::Clear()
{
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}



