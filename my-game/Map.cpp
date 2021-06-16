#include "Map.h"


LPTILESET getTileSetFromMin(unordered_map<int, LPTILESET>* tileset, int cell) {
	for (auto it = tileset->begin(); it != tileset->end(); ++it) {
		int id = it->first;
		if (cell > id) return it->second;
	}
	return tileset->begin()->second;
}


void Tileset::load(LPCWSTR path) {

}

void Layer::render(unordered_map<int, LPTILESET>* tileset) {
	//DebugOut(L"[INFOR] Render tile layer %s\n", ToLPCWSTR(this->name));



	for (int i = 0; i < width * height; i++) {
		int cell = data[i];
		if (cell > 0) {
			LPTILESET activeTileset = getTileSetFromMin(tileset, cell);

			Vector p;
			RECT r;

			int col = floor(i / width);
			int row = i % width;

			p.x = row * activeTileset->tileHeight;
			p.y = col * activeTileset->tileHeight; //calc from i, all size will be transform below

			int tilecol = floor(cell / activeTileset->imageWidth);
			int tileRow = cell % int(activeTileset->imageWidth);

			//int left = tileRow * activeTileset->tileWidth;
			//int top = tilecol * activeTileset->tileHeight;
			//int right = left + activeTileset->tileWidth;
			//int bottom = top + activeTileset->tileHeight; //calc from the cell

			r.top = ((cell - activeTileset->firstGrid) / activeTileset->columns) * activeTileset->tileHeight;
			r.left = ((cell - activeTileset->firstGrid) % activeTileset->columns) * activeTileset->tileWidth;
			r.bottom = r.top + activeTileset->tileHeight;
			r.right = r.left + activeTileset->tileWidth;

			if (activeTileset != NULL && activeTileset->texture != NULL) {
				draw(p, activeTileset->texture, r, 255);
			}
		}
	}

}

void Layer::draw(Vector p, LPDIRECT3DTEXTURE9 texture, RECT r, int opacity) {
	// transform every size to be one fixed size
	// also calc new position after scale
	// draw here
	CGame* game = CGame::GetInstance();

	D3DXVECTOR2 pos, scale;



	float width = r.right - r.left;
	float height = r.bottom - r.top;
	// assum that width = height;

	scale.x = (FIXED_TILE_SIZE / width);
	scale.y = (FIXED_TILE_SIZE / height);

	scale.x = 1;
	scale.y = 1;

	//p.x = 0;
	//p.y = 0;

	pos.x = p.x;
	pos.y = p.y;


	game->DrawWithScale(p, texture, r, 255, pos, scale);

	//game->Draw(p.x, p.y, texture, r.left, r.top, r.right, r.bottom, 255);
}

void Map::load(string path, vector<LPGAMEOBJECT>* obCollisions) {
	json mapData = ReadJsonFIle(ToLPCWSTR(path));
	this->data = mapData;

	//load all tileset first

	json tilesetData = mapData["tilesets"];
	for (json::iterator set = tilesetData.begin(); set != tilesetData.end(); ++set) {
		json data = set.value();
		Tileset* tile = new Tileset();
		int firstGrid = data["firstgid"];
		tile->firstGrid = data["firstgid"];
		tile->image = data["image"];
		tile->tileHeight = data["tileheight"];
		tile->tileWidth = data["tilewidth"];
		tile->imageHeight = data["imageheight"];
		tile->imageWidth = data["imagewidth"];
		tile->margin = data["margin"];
		tile->spacing = data["spacing"];
		tile->name = data["name"];
		tile->columns = data["columns"];
		//load texture here

		//LPCWSTR filePath = ToLPCWSTR(tile->image);  // have to fix here
		//LPCWSTR filePath = ToLPCWSTR("assets/map/World1-1/World1-1.png");  // have to fix here
		string directory;
		const size_t last_slash_idx = path.rfind('/');
		if (std::string::npos != last_slash_idx)
		{
			directory = path.substr(0, last_slash_idx);
		}

		LPCWSTR filePath = ToLPCWSTR(directory + "/" + tile->image);

		D3DXIMAGE_INFO info;
		HRESULT result = D3DXGetImageInfoFromFile(filePath, &info);
		if (result != D3D_OK)
		{
			DebugOut(L"[ERROR] Get world texture failed: %s\n", filePath);
			return;
		}

		LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();
		LPDIRECT3DTEXTURE9 textu;

		result = D3DXCreateTextureFromFileEx(
			d3ddv,								// Pointer to Direct3D device object
			filePath,							// Path to the image to load
			info.Width,							// Texture width
			info.Height,						// Texture height
			1,
			D3DUSAGE_DYNAMIC,
			D3DFMT_UNKNOWN,
			D3DPOOL_DEFAULT,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			D3DCOLOR_XRGB(255, 0, 255),		// transparentColor	
			&info,
			NULL,
			&textu);								// Created texture pointer

		if (result != D3D_OK)
		{
			OutputDebugString(L"[ERROR] CreateTextureFromFile failed\n");
			return;
		}

		tile->texture = textu;

		this->all_typeset[firstGrid] = tile;

	}

	//load all layer

	json layerData = mapData["layers"];
	for (json::iterator set = layerData.begin(); set != layerData.end(); ++set) {
		json data = set.value();
		Layer* layer = new Layer();
		string type = string(data["type"]);
		string name = string(data["name"]);
		if (type == "tilelayer") {
			int id = data["id"];
			int height = data["height"];
			int width = data["width"];
			layer->name = string(data["name"]);
			layer->opacity = data["opacity"];
			layer->type = TILE_LAYER;
			layer->x = float(data["x"]);
			layer->y = float(data["y"]);
			layer->width = width;
			layer->height = height;
			layer->data = new int[width * height];

			json grid = data["data"];
			int i = 0;

			for (json::iterator cell = grid.begin(); cell != grid.end(); ++cell) {
				int value = cell.value();
				layer->data[i] = value;
				i = i + 1;
			}

			this->all_layer[id] = layer;
		}

		else if (type == "objectgroup" && name == "RectCollision") {
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
		else if (type == "objectgroup" && name == "SelectionNode") {
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

				switch (fromNameToCode(name))
				{
				case 3:
					obj = new MisteryBox();
					break;
				case 4:
					obj = new Coin();
					break;
					break;
				case 5:
					obj = new Goomba();
					break;
				case 8:
					obj = new SelectionTree();
					break;
				case 9:
					obj = new IntroText();
					obj->type = type;
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
		else {
			delete layer;
		}
	}
}

void Map::render() {

	//CGame* game = CGame::GetInstance();
	//DebugOut(L"[INFO] render %s \n", ToLPCWSTR(id));
	//game->Draw(x, y, texture, left, top, right, bottom, alpha);



	/*for (auto it = all_layer.begin(); it != all_layer.end(); ++it) {
		int id = it->first;
		LPLAYER layer = it->second;
		layer->render(&all_typeset);
	}*/

	for (auto layer : all_layer) {
		layer.second->render(&all_typeset);
	}
};

