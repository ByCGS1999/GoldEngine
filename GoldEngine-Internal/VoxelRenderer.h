#pragma once
#include "VoxelTypedefs.h"
#include "Voxel.h"
#include "VoxelMatrix.h"
#include "Chunk.h"

class VoxelRenderer
{
public:

	// PROPERTIES \\

	Chunk* chunks;
	float voxelSize = 1.0f;
	Mesh voxelMesh;

	// CONSTRUCTOR \\

	VoxelRenderer(__int8 width, __int8 height, __int8 depth)
	{
		chunks = new Chunk(width, height, depth);
		RandomizeVoxelColors();
		voxelMesh = GenMeshCube(voxelSize, voxelSize, voxelSize);
	}


private:
	void UpdateVoxelMatrix(unsigned int* pos, Voxel value)
	{	
		chunks->voxelMatrix->Set(pos[0], pos[1], pos[2], value);
	}

	// METHODS \\
	
public:
	void Cull()
	{
		for (unsigned int x = 0; x < chunks->w; x++)
		{
			for (unsigned int y = 0; y < chunks->h; y++)
			{
				for (unsigned int z = 0; z < chunks->d; z++)
				{
					auto voxel = chunks->voxelMatrix->Get(x, y, z);

					voxel.SetRender(true);

					UpdateVoxelMatrix(new unsigned int[3] { x,y,z }, voxel);
				}
			}
		}
	}
	void RandomizeVoxelColors()
	{
		for (unsigned int x = 0; x < chunks->w; x++)
		{
			for (unsigned int y = 0; y < chunks->h; y++)
			{
				for (unsigned int z = 0; z < chunks->d; z++)
				{
					auto voxel = chunks->voxelMatrix->Get(x, y, z);

					unsigned int baseColor = 0x000000FF;

					unsigned int color = baseColor + (rand() % 0xFFFFFFFF + 0x000000FF);

					voxel.SetColor(color);

					UpdateVoxelMatrix(new unsigned int[3] { x, y, z }, voxel);
				}
			}
		}
	}
	void Render()
	{
		for (unsigned int x = 0; x < chunks->w; x++)
		{
			for (unsigned int y = 0; y < chunks->h; y++)
			{
				for (unsigned int z = 0; z < chunks->d; z++)
				{
					auto voxel = chunks->voxelMatrix->Get(x, y, z);

					unsigned int color = voxel.color;

					float _x, _y, _z;
					float newX, newY, newZ;

					_x = (float)x;
					_y = (float)y;
					_z = (float)z;

					newX = (float)(_x * voxelSize) + voxelSize;
					newY = (float)(_y * voxelSize) + voxelSize;
					newZ = (float)(_z * voxelSize) + voxelSize;

					if (voxel.render)
					{
						DrawModel(LoadModelFromMesh(voxelMesh), {_x,_y,_z}, voxelSize, GetColor(voxel.color));
					}
				}
			}
		}
	}
};