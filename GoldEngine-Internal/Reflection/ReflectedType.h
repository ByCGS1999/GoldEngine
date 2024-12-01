#pragma once

using namespace System;

namespace Engine::Reflectable
{
	typedef value struct ReflectedType
	{
	public:
		String^ assemblyName;
		String^ typeName;
		String^ nameSpace;
		String^ assemblyVersion;

	public:
		ReflectedType(String^ asmN, String^ tN, String^ nameSpace, String^ assemblyVersion)
		{
			this->assemblyName = asmN;
			this->assemblyVersion = assemblyVersion;
			this->typeName = tN;
			this->nameSpace = nameSpace;
		}
	};

	private ref class ReflectableManager
	{
	public:
		static System::Collections::Generic::List<System::Reflection::Assembly^>^ assemblies = gcnew System::Collections::Generic::List<System::Reflection::Assembly^>();

	public:
		static Reflection::Assembly^ GetAssemblyByName(String^);

		static Type^ GetTypeFromName(Reflection::Assembly^ assembly, String^ typeName, String^ nameSpace);
		static Type^ GetTypeFromName(String^ typeName, String^ nameSpace);
	};

	private ref class ReflectableType
	{
	private:
		System::Type^ typeReference;
		void SolveTypeRef();
		ReflectedType^ type;

	public:
		String^ reflectedData;

	public:
		ReflectableType(Type^);
		ReflectableType(String^);
		[Newtonsoft::Json::JsonConstructorAttribute]
		ReflectableType();
		Type^ getTypeReference();
		void SetType(Type^);
		void DeserializeType(String^);
		void DeserializeType();
	};
}