#pragma once

class Chunk
{
public:
	VoxelMatrix *voxelMatrix;
	Voxel* voxelArray;

public:
	__int8 w, h, d;

	Chunk(__int8 width, __int8 height, __int8 depth)
	{
		voxelMatrix = new VoxelMatrix(width, height, depth);
		voxelArray = new Voxel[width, height, depth];

		w = width;
		h = height;
		d = depth;

		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				for (int z = 0; z < d; z++)
				{
					Voxel *newVoxel = new Voxel(uint32_t(0xFFFFFFFF), true);
					voxelMatrix->Set(x, y, z, *newVoxel);
				}
			}
		}
	}

};