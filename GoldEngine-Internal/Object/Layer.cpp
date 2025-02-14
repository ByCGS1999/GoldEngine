#include "Layer.h"

using namespace Engine::Components;

namespace Engine::Components 
{
	Layer::Layer(int mask, System::String^ name)
	{
		this->layerMask = mask;
		this->layerName = name;
	}

	bool Layer::IsLayer(Layer^ layerB)
	{
		if ((this->layerName->Equals(layerB->layerName)) && (this->layerMask == layerB->layerMask))
			return true;
	
		return false;
	}

	void Layer::setLayerBlendFlags(unsigned int flags)
	{
		this->layerBlendFlags = flags;
	}

	unsigned int Layer::getLayerBlendFlags()
	{
		return this->layerBlendFlags;
	}
}