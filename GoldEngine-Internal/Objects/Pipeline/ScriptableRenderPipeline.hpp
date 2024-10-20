namespace Engine::Render
{
	public ref class ScriptableRenderPipeline abstract
	{
	public:
		ScriptableRenderPipeline()
		{
			Logging::Log("Creating new ScriptableRenderPipeline");

			Singleton<ScriptableRenderPipeline^>::Create(this);
			Singleton<HarmonyLib::Harmony^>::Instance->UnpatchAll("HarmonyInstance");
		}

	public:
		void ExecuteRenderWorkflow(Engine::Window^ windowHandle, Engine::Management::Scene^ scene)
		{
			PreFirstPassRender(scene);
			PostFirstPassRender();

			BeginDrawing();
			{
				OnRenderBegin();

				ClearBackground(GetColor(scene->skyColor));

				PreRenderFrame();
				{
					Engine::EngineObjects::Camera^ camera = ObjectManager::singleton()->GetMainCamera();

					if (camera == nullptr)
						return;

					bool is3DCamera = camera->is3DCamera();

					BeginMode3D(((Engine::EngineObjects::Native::NativeCamera3D*)camera->get())->get());

					int currentLayer = 1;

					render(currentLayer, scene);

					EndMode3D();
				}

				OnRenderEnd();

				rlImGuiBegin();

				for each (Engine::Management::MiddleLevel::SceneObject ^ obj in scene->GetRenderQueue())
				{
					if (obj != nullptr)
					{
						if (obj->GetReference() != nullptr)
						{
							obj->GetReference()->GameDrawImGUI();
						}
					}
				}

				//windowHandle->DrawImGui();

				rlImGuiEnd();
			}
			EndDrawing();

			PostRenderFrame();
		}

		void ExecuteRenderWorkflow_Editor(Engine::Window^ windowHandle, Engine::Management::Scene^ scene, RenderTexture renderTexture)
		{
			PreFirstPassRender(scene); // FIRSTPASS
			PostFirstPassRender();

			BeginDrawing();
			{
				BeginTextureMode(renderTexture);

				OnRenderBegin(); // BEGIN

				ClearBackground(GetColor(scene->skyColor));

				PreRenderFrame(); // PRE FRAME
				{
					Engine::EngineObjects::Camera^ camera = ObjectManager::singleton()->GetMainCamera();

					if (camera == nullptr)
						return;

					bool is3DCamera = camera->is3DCamera();

					BeginMode3D(((Engine::EngineObjects::Native::NativeCamera3D*)camera->get())->get());

					int currentLayer = 1;

					render(currentLayer, scene);

					EndMode3D();
				}

				OnRenderEnd();

				EndTextureMode();

				rlImGuiBegin();

				ImGui::Begin("DemoVer", (bool*)true, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking);
				{
					ImGui::SetWindowSize(ImVec2(285, 20), 0);
					ImGui::SetWindowPos(ImVec2(0, GetScreenHeight() - 25), 0);
					ImGui::TextColored(ImVec4(255, 255, 255, 255), ENGINE_VERSION);
					ImGui::End();
				}

				for each (Engine::Management::MiddleLevel::SceneObject ^ obj in scene->GetRenderQueue())
				{
					if (obj != nullptr)
					{
						if (obj->GetReference() != nullptr)
						{
							obj->GetReference()->GameDrawImGUI();
						}
					}
				}

				windowHandle->DrawImGui();

				rlImGuiEnd();
			}
			EndDrawing();

			PostRenderFrame();
		}

	protected:
		void render(int currentLayer, Engine::Management::Scene^ scene)
		{
			ObjectManager::singleton()->GetFirstObjectOfType<Engine::EngineObjects::LightManager^>()->LightUpdate();

			PreRenderObjects();

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
							PreRenderObject(reference);

							/*
							reference->GameDraw();
							*/
							#if PRODUCTION_BUILD == FALSE

							if (!EngineState::PlayMode)
							{
								reference->GameDrawGizmos();
							}

							#endif

							PostRenderObject();
						}
					}
				}

				Layer^ nextLayer = LayerManager::getNextHigherLayer(cL);

				if (nextLayer != nullptr)
					currentLayer = nextLayer->layerMask;
				else
					break;
			}

			PostRenderObjects();
		}

		void firstPassRender(int currentLayer) override
		{
			if (Engine::Scripting::ObjectManager::singleton() == nullptr || !Singleton<Engine::Management::Scene^>::Instantiated)
				return;

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

		void firstPassRender(int currentLayer, System::Collections::Generic::List< Engine::Internal::Components::Object^>^% objects)
		{
			if (Engine::Scripting::ObjectManager::singleton() == nullptr || !Singleton<Engine::Management::Scene^>::Instantiated)
				return;

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
							objects->Add(reference);
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

	public:
		// Render Per Frame
		virtual void PreRenderFrame() abstract;
		virtual void PostRenderFrame() abstract;

		// Rendering per object stack
		virtual void PreRenderObjects() abstract;
		virtual void PostRenderObjects() abstract;

		// Render Per Object
		virtual void PreRenderObject(Engine::Internal::Components::Object^) abstract;
		virtual void PostRenderObject() abstract;

		// Hooks at the beggining and end of the render process
		virtual void OnRenderBegin() abstract;
		virtual void OnRenderEnd() abstract;

		// FirstPass Rendering
		virtual void PreFirstPassRender(Engine::Management::Scene^) abstract;
		virtual void PostFirstPassRender() abstract;

		// For freeing resources and memory
		virtual void OnUnloadPipeline() abstract;
	};
}