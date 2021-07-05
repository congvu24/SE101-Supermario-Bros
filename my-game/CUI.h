#pragma once
#include "GameObject.h"
#include "Game.h"
#include <iostream>


class CUI {
public:
	CUI() {}
	void DrawText(string text, Vector p, Vector scale = Vector(1, 1));
	void DrawUI(string name, Vector p, Vector scale = Vector(1, 1));
	static void SaveStaticData(json data);
	static void ParseSpriteFromJson(LPCWSTR path);
	static void SetTexture(LPCWSTR path);
	static void AddSprite(string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);

	static json data;
	static LPDIRECT3DTEXTURE9 texture;
	static unordered_map<string, LPSPRITE> sprites;
};