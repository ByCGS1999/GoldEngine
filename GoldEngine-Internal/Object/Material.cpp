#include "Material.h"
#include "../SDK.h"

using namespace Engine::Components;

Engine::Components::Material::Material()
{
	MaterialProperties = gcnew System::Collections::Generic::Dictionary<System::String^, System::Object^>();

	MaterialProperties->Add("Shader", 0);
	MaterialProperties->Add("Base Color", gcnew Engine::Components::Color(255, 255, 255, 255));
	MaterialProperties->Add("MainTexture", 0);
	MaterialProperties->Add("NormalMap", 0);
}

bool Engine::Components::Material::AddProperty(System::String^ propName, System::Object^ arg)
{
	if (MaterialProperties->ContainsKey(propName))
	{
		MaterialProperties[propName] = arg;
		return true;
	}

	return false;
}

bool Engine::Components::Material::UpdateProperty(System::String^ propName, System::Object^ arg)
{
	if (MaterialProperties->ContainsKey(propName))
	{
		MaterialProperties[propName] = arg;
		return true;
	}

	return false;
}

bool Engine::Components::Material::RemoveProperty(System::String^ propName)
{
	if (MaterialProperties->ContainsKey(propName))
	{
		MaterialProperties->Remove(propName);
		return true;
	}

	return false;
}

System::Object^ Engine::Components::Material::GetComponent()
{
	return this;
}

System::Object^ Engine::Components::Material::GetMaterialProperty(System::String^ propName)
{
	if (MaterialProperties->ContainsKey(propName))
	{
		return MaterialProperties[propName];
	}

	return nullptr;
}

System::Object^ Engine::Components::Material::GetBaseColor()
{
	if (MaterialProperties->ContainsKey("Base Color"))
	{
		return MaterialProperties["Base Color"];
	}

	return nullptr;
}

unsigned int Engine::Components::Material::GetMainTexture()
{
	if (MaterialProperties->ContainsKey("MainTexture"))
	{
		return (int)MaterialProperties["MainTexture"];
	}

	return 0;
}

unsigned int Engine::Components::Material::GetNormalMap()
{
	if (MaterialProperties->ContainsKey("NormalMap"))
	{
		return (int)MaterialProperties["NormalMap"];
	}

	return 0;
}

unsigned int Engine::Components::Material::GetShader()
{
	if (MaterialProperties->ContainsKey("Shader"))
	{
		return (int)MaterialProperties["Shader"];
	}

	return 0;
}

generic <class T>
T Engine::Components::Material::GetMaterialProperty(System::String^ propName)
{
	if (MaterialProperties->ContainsKey(propName))
	{
		return (T)MaterialProperties[propName];
	}

	return T();
}

void Engine::Components::Material::SetMainTexture(int textureId)
{
	if (MaterialProperties->ContainsKey("MainTexture"))
	{
		MaterialProperties["MainTexture"] = textureId;
	}
}

void Engine::Components::Material::SetNormalMap(int textureId)
{
	if (MaterialProperties->ContainsKey("NormalMap"))
	{
		MaterialProperties["NormalMap"] = textureId;
	}
}

void Engine::Components::Material::SetShader(int shaderId)
{
	if (MaterialProperties->ContainsKey("Shader"))
	{
		MaterialProperties["Shader"] = shaderId;
	}
}

void Engine::Components::Material::SetBaseColor(Object^ color)
{
	if (MaterialProperties->ContainsKey("Base Color"))
	{
		MaterialProperties["Base Color"] = color;
	}
}