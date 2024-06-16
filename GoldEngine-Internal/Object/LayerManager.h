#pragma once

using namespace System;
using namespace Engine::Components;

namespace Engine::Scripting
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
		public ref class LayerManager abstract
	{
	private:
		static System::Collections::Generic::List<Layer^>^ layers;

	public:
		static void RegisterDefaultLayers()
		{
			layers = gcnew System::Collections::Generic::List<Layer^>();

			AddLayer(gcnew Layer(0, "EngineRoot"));
			AddLayer(gcnew Layer(1, "Geometry"));
			AddLayer(gcnew Layer(2, "PostFX"));
			AddLayer(gcnew Layer(3, "Transparent"));
		}

	public:
		static std::vector<std::string> getLayerNames()
		{
			std::vector<std::string> slayers = std::vector<std::string>();

			for (int x = 0; x < layers->Count; x++)
			{
				slayers.push_back(CastStringToNative(layers[x]->layerMask + " - " + layers[x]->layerName));
			}

			return slayers;
		}

		static std::vector<const char*> getLayerNames_char()
		{
			std::vector<const char*> slayers = std::vector<const char*>();

			for (int x = 0; x < layers->Count; x++)
			{
				slayers.push_back(CastStringToNative(layers[x]->layerMask + " - " + layers[x]->layerName).c_str());
			}

			return slayers;
		}

	public:
		static Layer^ GetLayerFromId(int id)
		{
			for each (Layer ^ l in layers)
			{
				if (l->layerMask == id)
				{
					return l;
				}
			}

			return nullptr;
		}

	public:
		static Layer^ GetLayerFromName(String^ id)
		{
			for each (Layer ^ l in layers)
			{
				if (l->layerName->Equals(id) || l->layerName->CompareTo(id) <= 0 || l->layerName == id)
				{
					return l;
				}
			}

			return nullptr;
		}


	public:
		static void AddLayer(Layer^ layer)
		{
			layers->Add(layer);
		}

	public:
		static Layer^ getNextHigherLayer(Layer^ currentLayer)
		{
			for each (Layer ^ l in layers)
			{
				if (l->layerMask > (currentLayer->layerMask))
				{
					return l;
				}
			}

			return nullptr;
		}

	public:
		static int getHigherLayer()
		{
			int layerLevel = int::MinValue;

			for each (Layer ^ l in layers)
			{
				if (l->layerMask > layerLevel)
				{
					layerLevel = l->layerMask;
				}
			}

			return layerLevel;
		}

		static int getLowerLayer()
		{
			int layerLevel = int::MaxValue;

			for each (Layer ^ l in layers)
			{
				if (l->layerMask < layerLevel)
				{
					layerLevel = l->layerMask;
				}
			}

			return layerLevel;
		}
	};
}