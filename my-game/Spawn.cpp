#include "Spawn.h"
#include "Vector.h"
#include <iostream>


LPDIRECT3DTEXTURE9 Spawn::texture = NULL;
unordered_map<string, LPSPRITE> Spawn::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> Spawn::all_animations; //save all animations
CAnimationSets Spawn::animations_set; //save all the animation sets
json Spawn::data = NULL;
json Spawn::spriteData = NULL;

Spawn::Spawn()
{
	SetState("running");
	isAllowCollision = false;
	isBlockPlayer = false;
	isUniversal = true;
}
