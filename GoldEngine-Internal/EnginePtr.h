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
		std::function<void(T)> onObjectChanged;

	public:
		EnginePtr()
		{
			ptrInstance = T();
			onObjectDeleted = NULL;
			onObjectChanged = NULL;
		}

		EnginePtr(T instance, std::function<void(T)> onDeleted = NULL, std::function<void(T)> onChanged = NULL)
		{
			this->ptrInstance = instance;
			this->onObjectDeleted = onDeleted;
			this->onObjectChanged = onChanged;
		}

		~EnginePtr()
		{
			if (onObjectDeleted != nullptr)
				onObjectDeleted(ptrInstance);
		}

		T& getInstance()
		{
			return ptrInstance;
		}

		void setInstance(const T& instance)
		{
			if(onObjectChanged != nullptr)
				onObjectChanged(this->ptrInstance);

			this->ptrInstance = instance;
		}

		void release() 
		{
			delete this;
		}

		/*
		EnginePtr(EnginePtr&& other) noexcept
			: ptrInstance(std::move(other.ptrInstance)), onObjectDeleted(std::move(other.onObjectDeleted)), onObjectChanged(std::move(other.onObjectChanged)) {
			other.ptrInstance = nullptr;
		}

		EnginePtr& operator=(EnginePtr&& other) noexcept {
			if (this != &other) {
				ptrInstance = std::move(other.ptrInstance);
				onObjectDeleted = std::move(other.onObjectDeleted);
				onObjectChanged = std::move(other.onObjectChanged);


				if (onObjectDeleted && ptrInstance) {
					onObjectDeleted(ptrInstance);
				}

				other.ptrInstance = T();
			}
			return *this;
		}
		*/
	};
}