#pragma once

struct Vector
{
	int x;
	int y;

	bool operator==(const struct Vector &rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}

	bool isEqual(const int x, const int y) const
	{
		return this->x == x && this->y == y;
	}
};