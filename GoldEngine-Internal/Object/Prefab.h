#pragma once

namespace Engine::Management
{
	public ref class Prefab
	{
	public:
		Dictionary<String^, Engine::Reflectable::ReflectableType^>^ instance = gcnew Dictionary<String^, Engine::Reflectable::ReflectableType^>();

	private:
		List<GameObject^>^ gameInstance = gcnew List<GameObject^>();

	public:
		Prefab(GameObject^ instance);

		[Newtonsoft::Json::JsonConstructorAttribute]
		Prefab();

		GameObject^ getInstance();
		Type^ getObjectType();

		List<GameObject^>^ getInstances();

		void DeserializeObject();

		static List<GameObject^>^ LoadPrefab(String^ prefabPath);
		static Prefab^ GetPrefab(String^ prefabPath);
	};
}