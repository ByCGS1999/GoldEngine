#include "Layer.h"

using namespace Engine::Components;

namespace Engine::Components 
{
	Layer::Layer(int mask, System::String^ name)
	{
		this->layerMask = mask;
		this->layerName = name;
	}
}