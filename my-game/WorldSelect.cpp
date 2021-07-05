#include <iostream>
#include <fstream>

#include "WorldSelect.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Test.h"
#include "Enemy.h"
#include "MisteryBox.h"
#include "Coin.h"
#include "Quadtree.h"
#include "MarioSelection.h"
#include "SelectNode.h"
#include "SelectionTree.h"
#include "DefineCharacter.h"


WorldSelect::WorldSelect(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CSelectScenceKeyHandler(this);
}

void WorldSelect::Load()
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

void WorldSelect::Update(DWORD dt)
{
	CScene::Update(dt);

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->camera;

	RECT base = { camera->cam_x, camera->cam_y, camera->cam_x + 800 ,camera->cam_y + 600 };
	Quadtree* quadtree = new Quadtree(1, new RECT(base));

	for (auto i = objects.begin(); i != objects.end(); i++) {
		quadtree->Insert(*i);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
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
	quadtree->Clear();
	delete quadtree;

}

void WorldSelect::Render()
{
	Camera* camera = CGame::GetInstance()->GetCurrentScene()->camera;

	map->render();
	player->Render();

	for (int i = 0; i < objects.size(); i++) {
		/*if (objects[i]->state != "hidden")*/
		objects[i]->Render();
	}

	if (currentNode != NULL && player != NULL)
		if (isMoving) {
			Vector centerObj = Vector(currentNode->p.x + currentNode->width / 2, currentNode->p.y + currentNode->height / 2);

			float destinationX = centerObj.x - player->width / 2;
			float destinationy = centerObj.y - player->height / 2;
			float distanceX = player->p.x - destinationX;
			float distanceY = player->p.y - destinationy;


			if (distanceX == 0 && distanceY == 0) isMoving = false;
			if (abs(distanceX) < 3.5) player->p.x -= distanceX;
			else player->p.x += 3.5 * (distanceX < 0 ? 1 : -1);
			if (abs(distanceY) < 3.5) player->p.y -= distanceY;
			else player->p.y += 3.5 * (distanceY < 0 ? 1 : -1);
		}
	CScene::Render();
}

/*
	Unload current scene
*/
void WorldSelect::Unload()
{
	player = NULL;
	//map->unload();
	CScene::Unload();
}

void CSelectScenceKeyHandler::OnKeyDown(int KeyCode)
{

	CGameObject* player = ((WorldSelect*)scence)->GetPlayer();
	WorldSelect* scene = ((WorldSelect*)scence);
	switch (KeyCode)
	{
	case DIK_S:

		if (scene->currentPortal != NULL) {
			scene->switchScene(stoi(scene->currentPortal->scene_id));
			//CGame::GetInstance()->SwitchScene(stoi(scene->currentPortal->scene_id));
		}
		break;
	}

}

void CSelectScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CGameObject* player = ((WorldSelect*)scence)->GetPlayer();

	//// disable control key when Mario die 
	if (player->GetState() == "die") return;
	if (game->IsKeyDown(DIK_RIGHT))
		player->SetState("running-right");
	else if (game->IsKeyDown(DIK_LEFT))
		player->SetState("running-left");
	else if (game->IsKeyDown(DIK_UP))
		player->SetState("running-up");
	else if (game->IsKeyDown(DIK_DOWN))
		player->SetState("running-down");
	else
		player->SetState("indie");
}



void  WorldSelect::_ParseSection_OBJECTS_FromJson(json allObjects) {
	for (json::iterator it = allObjects.begin(); it != allObjects.end(); ++it) {

		json data = it.value();

		string name = string(data["name"]); //object name;
		bool visible = bool(data["visible"]); //object name;

		CGameObject* obj = NULL;

		switch (fromNameToCode(name))
		{
		case ObjectType::Test:
			if (player != NULL)
			{
				return;
			}
			obj = new MarioSelection();
			obj->ParseFromJson(data); //remember to set position, animation_set in this function
			player = obj;
			break;
		case ObjectType::SelectionTree:
			if (visible != true)
				SelectionTree::SaveStaticData(data);
			break;
		default:
			break;
		}
		if (obj != NULL)
			objects.push_back(obj);
		else delete obj;
	}
}

void  WorldSelect::_ParseSection_MAP_FromJson(string mapPath) {
	vector<LPGAMEOBJECT> obCollision;

	this->map = new Map();
	map->load(mapPath, &obCollision, this);

	for (size_t i = 0; i < obCollision.size(); i++)
	{
		objects.push_back(obCollision[i]);
	}


}

void WorldSelect::addObject(LPGAMEOBJECT obj) {
	objects.push_back(obj);
}



void WorldSelect::ParseMapObject(json data, vector<LPGAMEOBJECT>* obCollisions) {
	string type = string(data["type"]);
	string name = string(data["name"]);

	if (type == "objectgroup" && name == "SelectionNode") {
		json objects = data["objects"];

		for (json::iterator objData = objects.begin(); objData != objects.end(); ++objData) {
			json value = objData.value();
			SelectNode* obj = new SelectNode();

			obj->ParseFromOwnJson();

			float width = float(value["width"]);
			float height = float(value["height"]);
			float x = float(value["x"]);
			float y = float(value["y"]);
			string nodeName = value["name"];

			json properties = value["properties"];
			for (json::iterator property = properties.begin(); property != properties.end(); ++property) {
				json data = property.value();
				string name = data["name"];
				if (name == "up") {
					obj->up = data["value"];
				}
				else if (name == "down") {
					obj->down = data["value"];
				}
				else if (name == "right") {
					obj->right = data["value"];
				}
				else if (name == "left") {
					obj->left = data["value"];
				}
			}

			obj->width = width;
			obj->height = height;
			obj->nodeName = nodeName;
			obj->name = "SelectionNode";
			obj->p = Vector(x, y);
			obCollisions->push_back(obj);
		}
	}
	else if (type == "objectgroup" && name == "SelectionPortal") {
		json objects = data["objects"];

		for (json::iterator objData = objects.begin(); objData != objects.end(); ++objData) {
			json value = objData.value();
			SelectPortal* obj = new SelectPortal();

			obj->ParseFromOwnJson();

			float width = float(value["width"]);
			float height = float(value["height"]);
			float x = float(value["x"]);
			float y = float(value["y"]);

			json properties = value["properties"];
			for (json::iterator property = properties.begin(); property != properties.end(); ++property) {
				json data = property.value();
				string name = data["name"];
				if (name == "scene_id") {
					obj->scene_id = data["value"];
				}
			}

			obj->width = width;
			obj->height = height;
			obj->name = "SelectPortal";
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
			case ObjectType::SelectionTree:
				obj = new SelectionTree();
				break;
			case ObjectType::IntroText:
				obj = new IntroText();
				obj->type = type;
				break;
			case ObjectType::Camera:
				camera->setCamPos(x, y);
				camera->cam_left_limit = x;
				camera->cam_top_limit = y;

				break;
			default:
				break;
			}

			if (obj != NULL) {
				obj->ParseFromOwnJson();
				obj->width = width;
				obj->height = height;
				obj->name = name;
				obj->p.x = x;
				obj->p.y = y;
				DebugOut(L"[INFO] Size Of Object: %s \n", IntToLPCWSTR(sizeof(*obj)));

				obCollisions->push_back(obj);
			}
			else delete obj;

		}


	}

}