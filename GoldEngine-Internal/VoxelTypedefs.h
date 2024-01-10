#pragma once


enum ColorMask { MASK_RED, MASK_GREEN, MASK_BLUE, MASK_ALPHA };

class Vector3Int
{
private:
	unsigned int x, y, z;

public:
	Vector3Int(unsigned int _x, unsigned int _y, unsigned int _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	Vector3Int(unsigned int *arr)
	{
		x = arr[0];
		y = arr[1];
		z = arr[2];
	}

	unsigned int GetX() 
	{
		return x;
	}

	unsigned int GetY()
	{
		return y;
	}

	unsigned int GetZ()
	{
		return z;
	}
};