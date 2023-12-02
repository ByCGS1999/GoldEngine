#pragma once
#include "Includes.h"

public class MaterialPack
{
public:
	unsigned int MaterialId;

public:
	Material MaterialReference;

public:
	MaterialPack(unsigned int id, Material ref)
	{
		MaterialId = id;
		MaterialReference = ref;
	}
};