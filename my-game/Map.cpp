#include "Map.h"


LPTILESET getTileSetFromMin(unordered_map<int, LPTILESET>* tileset, int cell) {
	for (auto it = tileset->begin(); it != tileset->end(); ++it) {
		int id = it->first;
		if (cell > id) return it->second;
	}
}


void Tileset::load(LPCWSTR path) {

}

void Layer::render(unordered_map<int, LPTILESET>* tileset) {
	//DebugOut(L"[INFOR] Render tile layer %s\n", ToLPCWSTR(this->name));

	CGame* game = CGame::GetInstance();

	for (int i = 0; i < width * height; i++) {
		int cell = data[i];
		if (cell > 0) {
			LPTILESET activeTileset = getTileSetFromMin(tileset, 100);

			int col = floor(i / width);
			int row = i % width;

			int x = row * 48;
			int y = col * 48; //calc from i;

			int tilecol = floor(cell / activeTileset->imageWidth);
			int tileRow = cell % int(activeTileset->imageWidth);

			int left = tileRow * 48;
			int top = tilecol * 48;
			int right = left + 48;
			int bottom = top + 48; //calc from the cell

			top = ((cell - activeTileset->firstGrid) / activeTileset->columns) * activeTileset->tileHeight;
			left = ((cell - activeTileset->firstGrid) % activeTileset->columns) * activeTileset->tileWidth;
			bottom = top + activeTileset->tileHeight;
			right = left + activeTileset->tileWidth;

			if (activeTileset != NULL && activeTileset->texture != NULL) {
				//DebugOut(L"[INFOR] Render texture %s\n");
				//LPDIRECT3DTEXTURE9 text =;

				game->Draw(x, y, activeTileset->texture, left, top, right, bottom, 255);
			}
		}
	}

}

void Map::load(LPCWSTR path) {
	json mapData = ReadJsonFIle(path);
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

		LPCWSTR filePath = L"assets/map/World1-1/World1-1.png";  // have to fix here

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

