#pragma once

struct Vector
{
	int x;
	int y;

	bool operator==(const struct Vector &rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}
};