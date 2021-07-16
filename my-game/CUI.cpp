#include "CUI.h"

unordered_map<string, LPSPRITE> CUI::sprites;
LPDIRECT3DTEXTURE9 CUI::texture = NULL;
json CUI::data = NULL;

void CUI::SaveStaticData(json data) {
	if (CUI::data == NULL) {
		CUI::data = data;
		LPCWSTR sprite = ToLPCWSTR(string(data["sprite"]));
		LPCWSTR texture = ToLPCWSTR(string(data["texture"]));

		CUI::SetTexture(texture);
		CUI::ParseSpriteFromJson(sprite);

		DebugOut(L"[INFO] Save static data complete!\n");

	}

}

void CUI::SetTexture(LPCWSTR path) {
	texture = CGame::LoadTexture(path);
}
void CUI::ParseSpriteFromJson(LPCWSTR path) {

	json sprite = ReadJsonFIle(path);
	json frames = sprite["frames"];


	for (json::iterator it = frames.begin(); it != frames.end(); ++it) {

		json data = it.value();
		string id = it.key();
		json frame = data["frame"];

		int l = frame["x"];
		int t = frame["y"];
		int r = l + frame["w"];

		int b = t + frame["h"];

		if (CUI::texture == NULL)
		{
			DebugOut(L"[ERROR] Texture ID %d not found!\n");
			return;
		}

		CUI::AddSprite(id, l, t, r, b, CUI::texture);
	}
	DebugOut(L"[INFO] Get Texture success : %s\n", path);
}

void CUI::AddSprite(string id, float left, float top, float right, float bottom, LPDIRECT3DTEXTURE9 tex) {
	LPSPRITE s = new CSprite(id, left, top, right, bottom, right - left, bottom - top, tex);
	CUI::sprites[id] = s;
}

void CUI::DrawText(string text, Vector p, Vector scale) {
	for (int i = 0; i < text.length(); i++) {
		std::string sprId = string(1, text[i]) == " " ? "space" : string(1, text[i]);
		try
		{
			LPSPRITE sprite = CUI::sprites.at(sprId);
			sprite->DrawPositionInCamera(p.x + i * 24 * scale.x, p.y, scale);
		}
		catch (const std::exception&)
		{
			DebugOut(L"[ERROR] Sprite not found: %s\n", ToLPCWSTR(to_string(text[i])));
		}
	}
}
void CUI::DrawUI(string name, Vector p, Vector scale) {
	try
	{
		LPSPRITE sprite = CUI::sprites.at(name);
		sprite->DrawPositionInCamera(p.x, p.y, scale);

	}
	catch (const std::exception&)
	{
		DebugOut(L"[ERROR] Sprite not found: %s\n", ToLPCWSTR(name));
	}
}