#pragma once

#include "../Includes.h"
#include "../GlIncludes.h"
#include "../CastToNative.h"
#include "ObjectType.h"
#include "ViewSpace.h"
#include "Layer.h"
#include "Transform.h"
#include "LayerManager.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"

using namespace System;

namespace Engine::Internal::Components
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class Transform
	{
	private:
		[Newtonsoft::Json::JsonPropertyAttribute]
			System::String^ uid;
	public:
		Engine::Internal::Components::Transform^ parent;
		// worldspace
		Engine::Components::Vector3^ position;
		Engine::Components::Vector3^ rotation;
		Engine::Components::Vector3^ scale;
		// localspace
		Engine::Components::Vector3^ localPosition;
		Engine::Components::Vector3^ localRotation;
		// CONSTANT VECTORS
		[Newtonsoft::Json::JsonIgnoreAttribute]
			const Engine::Components::Vector3^ forward = gcnew Engine::Components::Vector3(0, 0, 1);
		[Newtonsoft::Json::JsonIgnoreAttribute]
			const Engine::Components::Vector3^ top = gcnew Engine::Components::Vector3(0, 1, 0);
		[Newtonsoft::Json::JsonIgnoreAttribute]
			const Engine::Components::Vector3^ right = gcnew Engine::Components::Vector3(1, 0, 0);
		[Newtonsoft::Json::JsonIgnoreAttribute]
			const Engine::Components::Vector3^ backward = gcnew Engine::Components::Vector3(0, 0, -1);
		[Newtonsoft::Json::JsonIgnoreAttribute]
			const Engine::Components::Vector3^ bottom = gcnew Engine::Components::Vector3(0, -1, 0);
		[Newtonsoft::Json::JsonIgnoreAttribute]
			const Engine::Components::Vector3^ left = gcnew Engine::Components::Vector3(-1, 0, 0);

	public:
		Transform(Engine::Components::Vector3^ position, Engine::Components::Vector3^ rotation, Engine::Components::Vector3^ scale, Transform^ parent);

	public:
		void UpdateLocalPosition(Engine::Components::Vector3^ newLocalPosition)
		{
			position = localPosition - newLocalPosition;
		}

		String^ GetUID();

		void setParent(Transform^);
		Transform^ getParent();

		void SetParent(Transform^ parent) { return setParent(parent); }
		Transform^ GetParent() { return getParent(); }

		generic <class T>
		T GetObject();
		System::Object^ GetObject();

		void SetUID(String^ uid);
	};
}