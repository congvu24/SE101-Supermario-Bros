#include "Scence.h"
#include "Game.h"


LPDIRECT3DTEXTURE9 CScene::blackTexture = NULL;
CScene::CScene(int id, LPCWSTR filePath)
{
	this->id = id;
	this->sceneFilePath = filePath;
	this->camera = new Camera();
	animationDirection = UNACTIVE;
	lastTime = 0;
	animationDuration = 5000;
}

void CScene::LoadBlackTexture(LPCWSTR filePath) {
	if (CScene::blackTexture == NULL) {
		CScene::blackTexture = CGame::LoadTexture(filePath);
	}
}

void CScene::Render() {
	if (animationDirection == CLOSING) {
		//left to center
		CGame::GetInstance()->Draw(camera->cam_x - (camera->cam_width * (1 - animationProgress)), camera->cam_y, blackTexture, 0, 0, camera->cam_width, camera->cam_height, 255);
		//right to center
		CGame::GetInstance()->Draw(camera->cam_x + camera->cam_width - (camera->cam_width * animationProgress), camera->cam_y, blackTexture, 0, 0, camera->cam_width, camera->cam_height, 255);
		//top to center
		CGame::GetInstance()->Draw(camera->cam_x, camera->cam_y - (camera->cam_height * (1 - animationProgress)), blackTexture, 0, 0, camera->cam_width, camera->cam_height, 255);
		////bottom to center
		CGame::GetInstance()->Draw(camera->cam_x, camera->cam_y + camera->cam_height - (camera->cam_height * animationProgress), blackTexture, 0, 0, camera->cam_width, camera->cam_height, 255);
	}
}

void CScene::Update(DWORD dt) {
	if (animationDirection != UNACTIVE && animationDirection != DONE) {
		//float currentTime = GetTickCount64() - animationStartedTime;
		lastTime = lastTime + dt;
		animationProgress = (lastTime / animationDuration);
		if (animationProgress > 0.5) {
			animationDirection = DONE;
			if (nextScene != 0) {
				CGame::GetInstance()->SwitchScene(nextScene);
			}
		}
	}
}

void CScene::Unload() {
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->clear();
		delete objects[i];
	}
	objects.clear();
	
	// delete map data here;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void  CScene::_ParseSection_TEXTURES_FromJson(LPCWSTR filePath, int id) {

	DebugOut(L"[INFO] Start loading texture resources from : %s \n", filePath);

	int R = 0;
	int G = 0;
	int B = 0;

	CTextures::GetInstance()->Add(id, filePath, D3DCOLOR_XRGB(R, G, B));


}
void  CScene::_ParseSection_SPRITES_FromJson(LPCWSTR filePath, int textID) {

	json sprite = ReadJsonFIle(filePath);
	json frames = sprite["frames"];


	for (json::iterator it = frames.begin(); it != frames.end(); ++it) {

		json data = it.value();
		string id = it.key();
		json frame = data["frame"];


		int l = stoi(frame["x"].dump());
		int t = stoi(frame["y"].dump());
		int r = l + stoi(frame["w"].dump());
		int b = t + stoi(frame["h"].dump());

		LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(textID);
		if (tex == NULL)
		{
			DebugOut(L"[ERROR] Texture ID %d not found!\n", IntToLPCWSTR(textID));
			return;
		}

		CSprites::GetInstance()->Add(id, l, t, r, b, tex);
	}

}


void CScene::switchScene(int sence_id) {
	this->animationDirection = CLOSING;
	this->nextScene = sence_id;
}