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
		return Vector(a.x - b.y, a.y - b.y);
	}
	friend Vector operator* (Vector a, float x) {
		return Vector(a.x * x, a.y * x);
	}
	friend bool operator== (Vector a, Vector b) {
		return (a.x == b.x && a.y == b.y);
	}
};