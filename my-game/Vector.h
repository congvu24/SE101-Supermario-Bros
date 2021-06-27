#pragma once

class Vector {
public:
	float x = 0;
	float y = 0;
	Vector(float x = 0, float y = 0) {
		this->x = x;
		this->y = y;
	}
	friend Vector operator+ (Vector a, Vector b) {
		return Vector(a.x + b.x, a.y + b.y);
	}
	friend Vector operator- (Vector a, Vector b) {
		return Vector(a.x - b.x, a.y - b.y);
	}
	friend Vector operator* (Vector a, float x) {
		return Vector(a.x * x, a.y * x);
	}
	friend bool operator== (Vector a, Vector b) {
		return (a.x == b.x && a.y == b.y);
	}
	static Vector Normalize(Vector source)
	{
		float magnitude = (float)sqrt(source.x * source.x + source.y * source.y);
		return Vector((source.x / magnitude), (source.y / magnitude));
	}
};