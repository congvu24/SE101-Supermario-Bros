#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Test.h"
#include "Enemy.h"
#include "MisteryBox.h"
#include "Coin.h"
#include "Mushroom.h"
#include "Quadtree.h"
#include "Death.h"
#include "Venus.h"
#include "VenusBullet.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}


void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	json scene = ReadJsonFIle(sceneFilePath);


	DebugOut(L"[INFO] Scene : %s \n", ToLPCWSTR(scene["name"].dump()));

	//parse texture, sprite, animation

	json object = scene["object"];

	_ParseSection_OBJECTS_FromJson(object);


	string map = string(scene["map"]);
	DebugOut(L"[INFO] LOAD MAP : %s \n", ToLPCWSTR(map));

	_ParseSection_MAP_FromJson(map);

	animationDirection = UNACTIVE;
	animationStartedTime = GetTickCount64();
	animationProgress = 0;
	lastTime = 0;
}

void CPlayScene::Update(DWORD dt)
{
	CScene::Update(dt);
	Camera* camera = CGame::GetInstance()->GetCurrentScene()->camera;

	RECT base = { camera->cam_x, camera->cam_y, camera->cam_x + 800 ,camera->cam_y + 600 };
	Quadtree* quadtree = new Quadtree(1, new RECT(base));


	//DebugOut(L"[INFO] NUMBER OF OBJECT: %s \n", IntToLPCWSTR(objects.size()));


	for (auto i = objects.begin(); i != objects.end(); i++) {
		quadtree->Insert(*i);
	}
	int count = 0;

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (quadtree->IsContain(objects[i])) {
			count++;
			vector<CGameObject*>* return_objects_list = new vector<CGameObject*>();
			quadtree->Retrieve(return_objects_list, objects[i]);

			if (Test* v = dynamic_cast<Test*>(objects[i])) {
				objects[i]->Update(dt, return_objects_list);
			}

			else if (objects[i]->state != "hidden") {
				objects[i]->Update(dt, return_objects_list);
			}
			else {
				objects.erase(std::remove(objects.begin(), objects.end(), objects[i]), objects.end());
			}
			delete return_objects_list;
		}

		else if (objects[i]->isUniversal == true) {
			count++;
			objects[i]->Update(dt, &objects);
		}

	}
	quadtree->Clear();
	delete quadtree;

	//DebugOut(L"[INFO] UPDATE: %s \n", IntToLPCWSTR(count));

	if (player == NULL) return;

	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	if (camera->isCameraMoving == false) {
		camera->setCamPos(cx, camera->cam_y);
	}
}

bool comparePtrToNode(CGameObject* a, CGameObject* b) { return (a->renderOrder < b->renderOrder); }


void CPlayScene::Render()
{

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->camera;

	RECT* base = new RECT();
	base->left = camera->cam_x - 200; 
	base->top = camera->cam_y - 200; 
	base->right = camera->cam_x + 800 + 200;
	base->bottom = camera->cam_y + 600 + 200;

	Quadtree* quadtree = new Quadtree(5, base); // set the level to 5 to stop split function
	vector<CGameObject*>* return_objects_list = new vector<CGameObject*>();

	for (auto i = objects.begin(); i != objects.end(); i++) {
		quadtree->Insert(*i);
	}
	quadtree->Retrieve(return_objects_list, this->player);

	sort(return_objects_list->begin(), return_objects_list->end(), comparePtrToNode);

	for (int i = 0; i < return_objects_list->size(); i++) { // order < 1
		if (return_objects_list->at(i)->state != "hidden" && return_objects_list->at(i)->renderOrder < 1)
			return_objects_list->at(i)->Render();
	}

	map->render();  // render map here


	for (int i = 0; i < return_objects_list->size(); i++) { // order >= 1
		if (return_objects_list->at(i)->state != "hidden" && return_objects_list->at(i)->renderOrder >= 1)
			return_objects_list->at(i)->Render();
	}


	player->Render(); // render player 

	CScene::Render();  //render scene animation


	// clear data
	quadtree->Clear();
	return_objects_list->clear();
	delete return_objects_list;
	delete quadtree;
	//delete base;
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	player = NULL;
	//map->unload();
	CScene::Unload();
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{

	CGameObject* player = ((CPlayScene*)scence)->GetPlayer();
	((Character*)player)->ProcessKeyboard(CGame::GenerateKeyboardEvent(KeyCode));

	/*switch (KeyCode)
	{
	case DIK_SPACE:
		player->SetState("jumping");
		DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
		break;
	}*/

}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{

	CGameObject* player = ((CPlayScene*)scence)->GetPlayer();

	((Character*)player)->ProcessKeyboard(CGame::GenerateKeyboardEvent(KeyCode, false, true));


	/*switch (KeyCode)
	{
	case DIK_SPACE:
		player->SetState("jumping");
		DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
		break;
	}*/

}


void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CGameObject* player = ((CPlayScene*)scence)->GetPlayer();



	std::vector<int> UnOrderProcessKey = { DIK_A, DIK_S };
	std::vector<int> OrderProcessKey = { DIK_LEFT,DIK_RIGHT, DIK_DOWN, DIK_UP };


	for (int i = 0; i < UnOrderProcessKey.size(); i++) {
		if (game->IsKeyDown(UnOrderProcessKey[i])) {
			((Character*)player)->ProcessKeyboard(CGame::GenerateKeyboardEvent(UnOrderProcessKey[i], true));
		}
	}

	for (int i = 0; i < OrderProcessKey.size(); i++) {
		if (game->IsKeyDown(OrderProcessKey[i])) {
			((Character*)player)->ProcessKeyboard(CGame::GenerateKeyboardEvent(OrderProcessKey[i], true));
			return;
		}
	}


	if (game->IsKeyDown(DIK_R))
		((CPlayScene*)scence)->restart();
	else
		player->SetState("indie");


	//CGame::GenerateKeyboardEvent(UnOrderProcessKey[i], true);

	//// disable control key when Mario die 

	// move camera
	if (game->IsKeyDown(DIK_J))
		((CPlayScene*)scence)->moveCamera(LEFT);
	if (game->IsKeyDown(DIK_K))
		((CPlayScene*)scence)->moveCamera(DOWN);
	if (game->IsKeyDown(DIK_I))
		((CPlayScene*)scence)->moveCamera(UP);
	if (game->IsKeyDown(DIK_L))
		((CPlayScene*)scence)->moveCamera(RIGHT);
}

void CPlayScene::restart() {
	//this->player->SetPosition(100, 1000);
	//this->camera->move_x = 0;
	//this->camera->move_y = 0;
	//this->camera->isCameraMoving = false;
	CGame::GetInstance()->Restart();
}

void CPlayScene::moveCamera(CameraMoveDirection direction) {
	camera->isCameraMoving = true;
	if (direction == UP) {
		camera->move(0, -20);
	}
	else if (direction == DOWN) {
		camera->move(0, 20);
	}
	else if (direction == RIGHT) {
		camera->move(20, 0);
	}
	else if (direction == LEFT) {
		camera->move(-20, 0);
	}
}

void  CPlayScene::_ParseSection_OBJECTS_FromJson(json allObjects) {
	for (json::iterator it = allObjects.begin(); it != allObjects.end(); ++it) {

		json data = it.value();

		string name = string(data["name"]); //object name;
		bool visible = bool(data["visible"]); //object name;

		CGameObject* obj = NULL;

		switch (fromNameToCode(name))
		{
		case 1:
			if (player != NULL)
			{
				DebugOut(L"[ERROR] MARIO object was created before!\n");
				return;
			}
			obj = new Test();
			obj->ParseFromJson(data); //remember to set position, animation_set in this function
			player = obj;
			break;

		case 3:
			if (visible != true)
				MisteryBox::SaveStaticData(data);
		case 4:
			if (visible != true)
				Coin::SaveStaticData(data);
			break;
		case 5:
			if (visible != true)
				Goomba::SaveStaticData(data);
			break;
		case 6:
			if (visible != true)
				RectPlatform::SaveStaticData(data);
			break;
		case 7:
			if (visible != true)
				Leaf::SaveStaticData(data);
			break;
		case 11:
			if (visible != true)
				Mushroom::SaveStaticData(data);
			break;
		case 12:
			if (visible != true)
				Venus::SaveStaticData(data);
			break;
		case 13:
			if (visible != true)
				VenusBullet::SaveStaticData(data);
			break;
		case 14:
			if (visible != true)
				Koopas::SaveStaticData(data);
			break;
		default:
			break;
		}
		if (obj != NULL)
			objects.push_back(obj);
		else delete obj;
	}

}

void  CPlayScene::_ParseSection_MAP_FromJson(string mapPath) {
	vector<LPGAMEOBJECT> obCollision;

	this->map = new Map();
	map->load(mapPath, &obCollision, this);

	for (size_t i = 0; i < obCollision.size(); i++)
	{
		objects.push_back(obCollision[i]);
	}
}

void CPlayScene::addObject(LPGAMEOBJECT obj) {
	objects.push_back(obj);
}


void CPlayScene::ParseMapObject(json data, vector<LPGAMEOBJECT>* obCollisions) {
	string type = string(data["type"]);
	string name = string(data["name"]);

	if (type == "objectgroup" && name == "RectCollision") {
		json objects = data["objects"];

		for (json::iterator objData = objects.begin(); objData != objects.end(); ++objData) {
			json value = objData.value();
			LPGAMEOBJECT obj = new Collision();

			obj->ParseFromOwnJson();

			float width = float(value["width"]);
			float height = float(value["height"]);
			float x = float(value["x"]);
			float y = float(value["y"]);


			obj->width = width;
			obj->height = height;
			obj->name = "RectCollision";
			obj->p = Vector(x, y);

			obCollisions->push_back(obj);
		}
	}
	else if (type == "objectgroup" && name == "MiniPortal") {
		json objects = data["objects"];

		for (json::iterator objData = objects.begin(); objData != objects.end(); ++objData) {
			json value = objData.value();
			MiniPortal* obj = new MiniPortal();

			obj->ParseFromOwnJson();

			float width = float(value["width"]);
			float height = float(value["height"]);
			float x = float(value["x"]);
			float y = float(value["y"]);
			string portalName = value["name"];
			string type = value["type"];

			/*float can = float(value["y"]);
			float y = float(value["y"]);*/

			if (type == "Out") {
				json properties = value["properties"];
				for (json::iterator property = properties.begin(); property != properties.end(); ++property) {
					json data = property.value();
					string name = data["name"];
					if (name == "CameraLeftTopLimitX") {
						obj->camera_x = float(data["value"]);
					}
					else if (name == "CameraLeftTopLimitY") {
						obj->camera_y = float(data["value"]);
					}
				}
			}


			obj->width = width;
			obj->height = height;
			obj->portalName = portalName;
			obj->name = "MiniPortal";
			obj->type = type;
			obj->p = Vector(x, y);

			obCollisions->push_back(obj);
		}
	}
	else if (type == "objectgroup" && name == "RectPlatform") {
		json objects = data["objects"];

		for (json::iterator objData = objects.begin(); objData != objects.end(); ++objData) {
			json value = objData.value();
			LPGAMEOBJECT obj = new RectPlatform();

			obj->ParseFromOwnJson();

			float width = float(value["width"]);
			float height = float(value["height"]);
			float x = float(value["x"]);
			float y = float(value["y"]);



			obj->width = width;
			obj->height = height;
			obj->name = name;
			obj->p = Vector(x, y);

			obCollisions->push_back(obj);
		}
	}
	else if (type == "objectgroup" && name == "Death") {
		json objects = data["objects"];

		for (json::iterator objData = objects.begin(); objData != objects.end(); ++objData) {
			json value = objData.value();
			LPGAMEOBJECT obj = new Death();

			obj->ParseFromOwnJson();

			float width = float(value["width"]);
			float height = float(value["height"]);
			float x = float(value["x"]);
			float y = float(value["y"]);


			obj->width = width;
			obj->height = height;
			obj->name = "Death";
			obj->p = Vector(x, y);

			obCollisions->push_back(obj);
		}
	}
	else if (type == "objectgroup" && name != "RectCollision") {
		DebugOut(L"[INFO] Load name: %s \n", ToLPCWSTR(name));

		json objects = data["objects"];

		for (json::iterator objData = objects.begin(); objData != objects.end(); ++objData) {
			json value = objData.value();

			float width = float(value["width"]);
			float height = float(value["height"]);
			float x = float(value["x"]);
			float y = float(value["y"]);
			string type = value["type"];

			CGameObject* obj = NULL;
			Camera* camera = CGame::GetInstance()->GetCurrentScene()->getCamera();

			switch (fromNameToCode(name))
			{
			case 3:
				obj = new MisteryBox();
				break;
			case 4:
				obj = new Coin();
				break;
			case 5:
				obj = new Goomba();
				break;
			case 12:
				obj = new Venus();
				break;
			case 14:
				obj = new Koopas();
				break;
			case 9999:
				camera->setCamPos(x, y);
				camera->cam_x_limit = x;
				camera->cam_y_limit = y;
				break;
			default:
				break;
			}

			if (obj != NULL) {
				obj->width = width;
				obj->height = height;
				obj->name = name;
				obj->p.x = x;
				obj->p.y = y;
				obj->ParseFromOwnJson();
				DebugOut(L"[INFO] Size Of Object: %s \n", IntToLPCWSTR(sizeof(*obj)));

				obCollisions->push_back(obj);
			}
			else delete obj;

		}


	}
}

void CPlayScene::GameOver() {
	switchScene(3);
}

bool CPlayScene::IsPlayer(LPGAMEOBJECT obj) {
	if (Test* player = dynamic_cast<Test*>(obj))  return true;
	return false;
}