#include <iostream>
#include <math.h>
#include <algorithm>
using namespace std;

enum eDirection {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

struct Rect
{
	float x;
	float y;

	float width;
	float height;

	float vx;
	float vy;
};

bool isColliding(const Rect& object, const Rect& other);


Rect getSweptBroadphaseRect(const Rect& object);

float SweptAABBCalc(const Rect& object, const Rect& other, eDirection& result);
