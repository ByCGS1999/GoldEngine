#pragma once

class Chunk
{
public:
	VoxelMatrix* voxelMatrix;
	Voxel* voxelArray;

	float voxelSize;

private:
	int CHUNK_SIZE;

public:
	__int8 w, h, d;

	Chunk(__int8 width, __int8 height, __int8 depth, float voxelSize)
	{
		this->voxelMatrix = new VoxelMatrix(width, height, depth);
		this->voxelArray = new Voxel[width, height, depth];
		this->voxelSize = voxelSize;

		w = width;
		h = height;
		d = depth;

		CHUNK_SIZE = w * h * d;

		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				for (int z = 0; z < d; z++)
				{
					Voxel* newVoxel = new Voxel(uint32_t(0xFFFFFFFF), true);
					voxelMatrix->Set(x, y, z, *newVoxel);
				}
			}
		}
	}

	void Render()
	{
		if (voxelMatrix != nullptr)
		{
			auto matrix = voxelMatrix->GetRaw();

			bool lDefault = true;

			for (int x = 0; x < w; x++)
			{
				for (int y = 0; y < h; y++)
				{
					for (int z = 0; z < d; z++)
					{
						Voxel v = voxelMatrix->Get(x, y, z);

						if (v.GetRender() == false)
							continue;

						if ((x != 0 && x != w-1) && (y != 0 && y != h - 1) && (z != 0 && z != d - 1))
							continue;

						DrawCube({ (float)x,(float)y,(float)z }, voxelSize, voxelSize, voxelSize, GetColor(v.GetColor()));
					}
				}
			}
		}
	}
};