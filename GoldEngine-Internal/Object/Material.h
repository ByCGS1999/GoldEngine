#pragma once

namespace Engine::Components
{
	public ref class Material
	{
	public:
		System::Collections::Generic::Dictionary<System::String^, System::Object^>^ MaterialProperties;

	public:
		Material();


	public:
		bool AddProperty(System::String^ propertyName, System::Object^ value);
		bool UpdateProperty(System::String^ propertyName, System::Object^ value);
		bool RemoveProperty(System::String^ name);

		generic <class T>
		T GetMaterialProperty(System::String^ name);

		System::Object^ GetMaterialProperty(System::String^ name);
		System::Object^ GetBaseColor();
		unsigned int GetMainTexture();
		unsigned int GetNormalMap();
		unsigned int GetShader();

		void SetShader(int shaderId);
		void SetMainTexture(int textureId);
		void SetNormalMap(int textureId);
		void SetBaseColor(Object^ color);

		Object^ GetComponent();
	};
}