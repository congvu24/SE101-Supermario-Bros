#pragma once

#include <d3dx9.h>
#include "KeyEventHandler.h"
#include "Textures.h"
#include "Camera.h"


enum AnimationDirection {
	CLOSING,
	OPENING,
	UNACTIVE,
	DONE
};

class CScene
{
protected:
	CKeyEventHandler* key_handler;
	int id;
	LPCWSTR sceneFilePath;

public:
	CScene(int id, LPCWSTR filePath);
	Camera* camera;
	int nextScene = 0;
	vector<LPGAMEOBJECT> objects;
	static LPDIRECT3DTEXTURE9 blackTexture; // texture to render opening and closing animation
	float animationDuration;
	float animationProgress; //  = (getTickcount64 - animationStartedTime) / animationDuration;
	float animationStartedTime; // get tick count 64 when start loading;
	float lastTime;
	AnimationDirection animationDirection;

	CKeyEventHandler* GetKeyEventHandler() { return key_handler; }
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
	virtual void addObject(LPGAMEOBJECT obj) = 0;

	virtual void _ParseSection_TEXTURES_FromJson(LPCWSTR filePath, int id);
	virtual void _ParseSection_SPRITES_FromJson(LPCWSTR filePath, int textID);
	virtual void ParseMapObject(json data, vector<LPGAMEOBJECT>* obCollisions) = 0;
	virtual void switchScene(int sence_id);

	Camera* getCamera() {
		return this->camera;
	}

	static void LoadBlackTexture(LPCWSTR filePath);
};
typedef CScene* LPSCENE;


class CScenceKeyHandler : public CKeyEventHandler
{
protected:
	CScene* scence;

public:
	virtual void KeyState(BYTE* states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	CScenceKeyHandler(CScene* s) :CKeyEventHandler() { scence = s; }
};