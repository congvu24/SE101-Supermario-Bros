#include "SweptAABB.h"

bool isColliding(const Rect& object, const Rect& other)
{
	float left = other.x - (object.x + object.width);
	float top = (other.y + other.height) - object.y;
	float right = (other.x + other.width) - object.x;
	float bottom = other.y - (object.y + object.height);

	// mình xét ngược lại cho nhanh hơn
	return !(left > 0 || right < 0 || top < 0 || bottom > 0);
}

Rect getSweptBroadphaseRect(const Rect& object)
{
	float x = object.vx > 0 ? object.x : object.x + object.vx;
	float y = object.vy > 0 ? object.y : object.y + object.vy;
	float w = object.width + abs(object.vx);
	float h = object.height + abs(object.vy);

	return Rect{ x, y, w, h };
}

float SweptAABBCalc(const Rect& object, const Rect& other, eDirection& result)
{
	float dxEntry, dxExit;
	float dyEntry, dyExit;
	float txEntry, tyEntry, txExit, tyExit;

	Rect rect = getSweptBroadphaseRect(object);
	if (!isColliding(rect, other))
	{
		return 1.0f;
	}

	// tìm khoảng cách các cạnh tương ứng
	if (object.vx > 0.0f)
	{
		dxEntry = other.x - (object.x + object.width);
		dxExit = (other.x + other.width) - object.x;
	}
	else
	{
		dxEntry = (other.x + other.width) - object.x;
		dxExit = other.x - (object.x + object.width);
	}

	if (object.vy > 0.0f)
	{
		dyEntry = other.y - (object.y + object.height);
		dyExit = (other.y + other.height) - object.y;
	}
	else
	{
		dyEntry = (other.y + other.height) - object.y;
		dyExit = other.y - (object.y + object.height);
	}

	dxEntry = abs(dxEntry);
	dyEntry = abs(dyEntry);
	dxExit = abs(dxExit);
	dyExit = abs(dyExit);
	// tính thời gian từ khoảng cách tính được và vận tốc của đối tượng
	// vận tốc này là trong 1 frame hình nha
	if (object.vx == 0.0f)
	{
		// đang đứng yên thì bằng vô cực (chia cho  0)
		txEntry = -std::numeric_limits<float>::infinity();
		txExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		txEntry = dxEntry / object.vx;
		txExit = dxExit / object.vx;
	}

	if (object.vy == 0.0f)
	{
		tyEntry = -std::numeric_limits<float>::infinity();
		tyExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		tyEntry = dyEntry / object.vy;
		tyExit = dyExit / object.vy;
	}

	// thời gian va chạm là thời gian lớn nhất của 2 trục (2 trục phải cùng tiếp xúc thì mới va chạm)
	float entryTime = max(txEntry, tyEntry);
	// thời gian hết va chạm là thời gian của 2 trục, (1 cái ra khỏi là object hết va chạm)
	float exitTime = min(txExit, tyExit);

	// kiểm tra xem có thể va chạm không, mình xét ngược lại cho nhanh
	if (entryTime > exitTime || (txEntry < 0.0f && tyEntry < 0.0f) || txEntry > 1.0f || tyEntry > 1.0f)
	{
		return 1.0f;
	}

	// lấy hướng va chạm
	if (txEntry > tyEntry)
	{
		if (dxEntry > 0.0f)
		{
			result = eDirection::RIGHT;
		}
		else
		{
			result = eDirection::LEFT;
		}
	}
	else
	{
		if (dyEntry > 0.0f)
		{
			result = eDirection::UP;
		}
		else
		{
			result = eDirection::DOWN;
		}
	}

	// có thì lấy thời gian
	return entryTime;
}