#include "Layer.h"

using namespace Engine::Internal::Components;

Layer::Layer(int mask, System::String^ name)
{
	this->layerMask = mask;
	this->layerName = name;
}