#pragma once
#include "Includes.h"

public class ShaderPack
{
public:
	unsigned int shaderId;

public:
	Shader shaderReference;

public:
	ShaderPack(unsigned int id, Shader ref)
	{
		shaderId = id;
		shaderReference = ref;
	}
};
