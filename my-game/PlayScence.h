#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Map.h"

enum CameraMoveDirection {
	UP,
	DOWN,
	RIGHT,
	LEFT
};

class CPlayScene : public CScene
{
protected:
	CGameObject* player;					// A play scene has to have player, right? 

	vector<LPGAMEOBJECT> objects;

	void _ParseSection_TEXTURES_FromJson(LPCWSTR filePath, int id);
	void _ParseSection_SPRITES_FromJson(LPCWSTR filePath, int textID);
	void _ParseSection_ANIMATIONS_FromJson(LPCWSTR filePath);
	void _ParseSection_ANIMATION_SETS_FromJson(LPCWSTR filePath);
	void _ParseSection_OBJECTS_FromJson(json data);
	void _ParseSection_MAP_FromJson(string mapPath);


public:
	Map* map;
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	LPMAP GetMap() { return this->map; };
	void restart();
	void moveCamera(CameraMoveDirection);

	CGameObject* GetPlayer() { return player; }

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

