#pragma once

#include "functional"

namespace Engine::Native
{
	template <typename T>
	class EnginePtr
	{
	private:
		T ptrInstance;
		std::function<void(T)> onObjectDeleted;

	public:
		EnginePtr(T instance, std::function<void(T)> onDeleted)
		{
			this->ptrInstance = instance;
			this->onObjectDeleted = onDeleted;
		}

		~EnginePtr()
		{
			if (onObjectDeleted)
			{
				onObjectDeleted(ptrInstance);
			}
		}

		T& getInstance()
		{
			return ptrInstance;
		}

		void setInstance(const T& instance)
		{
			this->ptrInstance = instance;
		}

		EnginePtr(EnginePtr&& other) noexcept
			: ptrInstance(std::move(other.ptrInstance)), onObjectDeleted(std::move(other.onObjectDeleted)) {
			other.ptrInstance = nullptr;
		}

		EnginePtr& operator=(EnginePtr&& other) noexcept {
			if (this != &other) {
				if (onObjectDeleted && ptrInstance) {
					onObjectDeleted(ptrInstance);
				}
				ptrInstance = std::move(other.ptrInstance);
				onObjectDeleted = std::move(other.onObjectDeleted);
				other.ptrInstance = nullptr;
			}
			return *this;
		}
	};
}