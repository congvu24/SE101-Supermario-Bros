#include "Effect.h"


LPDIRECT3DTEXTURE9 Effect::texture = NULL;
unordered_map<string, LPSPRITE> Effect::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> Effect::all_animations; //save all animations
CAnimationSets Effect::animations_set; //save all the animation sets
json Effect::data = NULL;

Effect::Effect(string type, int time) {
	this->time = time;
	this->type = type;
	this->ParseFromOwnJson();
}

void Effect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (time >= 0) {
		time = time - dt;
	}
	if (time <= 0) SetState("hidden");
}

void Effect::Render() {
	MapEntity::Render();
}