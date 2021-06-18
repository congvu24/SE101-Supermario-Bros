#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "Utils.h"
#include "Game.h"
#include "textures.h"

CTextures* CTextures::__instance = NULL;

CTextures::CTextures()
{

}

CTextures* CTextures::GetInstance()
{
	if (__instance == NULL) __instance = new CTextures();
	return __instance;
}

void CTextures::Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor)
{
	
	textures[id] = CGame::LoadTexture(filePath);

	DebugOut(L"[INFO] Texture loaded Ok: id=%d, %s\n", id, filePath);
}

LPDIRECT3DTEXTURE9 CTextures::Get(unsigned int id)
{
	return textures[id];
}

/*
	Clear all loaded textures
*/
void CTextures::Clear()
{
	for (auto x : textures)
	{
		LPDIRECT3DTEXTURE9 tex = x.second;
		if (tex != NULL) tex->Release();
	}

	textures.clear();
}



