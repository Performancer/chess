#include "state.h"

char getType(char tile)
{
	return (tile | BLACK) ^ BLACK;
}

bool getColor(char tile)
{
	return  (tile & BLACK) == BLACK;
}

bool isAlly(char a, char b)
{
	return (a & BLACK) == (b & BLACK);
}
