#pragma once

namespace Engine::Interfaces
{
	generic <class T>
	public interface class IInstantiable
	{
	public:
		static T Create() { return T(); }
		static T New() { return T(); }
	};
}