#pragma once


struct VoxelMatrix
{
private:
	Voxel data[1][1][1] =
	{
		{ { Voxel() }}
	};
	const __int64 defaultVoxelValue = 0xFFFFFFFF6401;

public:
	VoxelMatrix(unsigned int w, unsigned int h, unsigned int d)
	{
		data[w][h][d];
		
		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				for (int z = 0; z < d; z++)
				{
					data[x][y][z] = Voxel();
				}
			}
		}
	}

	Voxel Get(unsigned int x, unsigned int y, unsigned int z)
	{
		if (sizeof(data) <= 0)
		{
			printf("Voxel Matrix size equals 0");
			return Voxel();
		}

		return data[x][y][z];
	}

	void Set(unsigned int x, unsigned int y, unsigned int z, Voxel value)
	{
		if (sizeof(data) <= 0)
		{
			printf("Voxel Matrix size equals 0");
			return;
		}

		data[x][y][z] = value;
	}

	auto GetRaw()
	{
		return data;
	}
};