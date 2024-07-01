public ref class ScriptableRenderPipeline
{
public:
	ScriptableRenderPipeline()
	{
		Singleton<ScriptableRenderPipeline^>::Create(this);
	}
	
private:
	void render(int currentLayer)
	{
		Singleton<Engine::EngineObjects::LightManager^>::Instance->LightUpdate();
		Engine::Management::Scene^ scene = Singleton<Engine::Management::Scene^>::Instance;

		while (currentLayer != LayerManager::getHigherLayer())
		{
			Layer^ cL = LayerManager::GetLayerFromId(currentLayer);

			if (cL == nullptr)
				break;

			for each (Engine::Management::MiddleLevel::SceneObject ^ sceneObject in scene->GetRenderQueue())
			{
				if (scene->sceneLoaded())
				{
					Engine::Internal::Components::Object^ reference = (Engine::Internal::Components::Object^)sceneObject->GetReference();

					if (reference->layerMask = cL)
					{
						reference->Draw();
						reference->DrawGizmo();
					}
				}
			}

			Layer^ nextLayer = LayerManager::getNextHigherLayer(cL);

			if (nextLayer != nullptr)
				currentLayer = nextLayer->layerMask;
			else
				break;
		}
	}
};