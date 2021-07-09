#pragma once
#include <iostream>
#include <string.h>
#include <d3dx9.h>
#include "Game.h"
#include "Vector.h"
#include "GameObject.h"
#include "Collision.h"
#include "MisteryBox.h"
#include "Coin.h"
#include "Goomba.h"
#include "RectPlatform.h"
#include "MiniPortal.h"
#include "SelectNode.h"
#include "SelectPortal.h"
#include "SelectionTree.h"
#include "IntroText.h"
#include "IntroCursor.h"



#include "Utils.h"

using namespace std;

#define FIXED_TILE_SIZE 20


enum layer_type
{
	OBJECT_GROUP,
	TILE_LAYER
};


class Tileset {
public:
	LPDIRECT3DTEXTURE9 texture;
	float tileHeight;
	float tileWidth;
	float spacing;
	float margin;
	int firstGrid;
	float imageHeight;
	float imageWidth;
	int columns;
	string name;
	string image;

	Tileset() {  };
	void load(LPCWSTR path);
	void unload();
};
typedef Tileset* LPTILESET;


class Layer {
public:

	layer_type type;
	int* data;
	float opacity;
	string name;
	float x;
	float y;
	int width;
	int height;

	Layer() {  };
	void render(unordered_map<int, LPTILESET>* tileset);
	void draw(Vector p, LPDIRECT3DTEXTURE9 texture, Rect r, int opacity);
	void unload();
};

typedef Layer* LPLAYER;



class Map {
public:
	bool isRendered = false;
	json data;
	string pathData;
	unordered_map<int, LPTILESET> all_typeset;
	unordered_map<int, LPLAYER> all_layer;

	Map() {};
	void load(string path, vector<LPGAMEOBJECT>* obCollisions, LPSCENE scene);
	void unload();
	void render();
};


typedef Map* LPMAP;
