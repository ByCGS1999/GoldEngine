#pragma once
using namespace System;

namespace Engine::Scripting
{
	public ref class ScriptBehaviour
	{
	public:
		virtual void Start() {}
	public:
		virtual void Update() {}
	public:
		virtual void FixedUpdate() {}
	public:
		virtual void OnCollision() {}
	};
}