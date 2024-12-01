namespace Engine::Render
{
	void UnloadRenderTextureDepthTex(RenderTexture2D target)
	{
		if (target.id > 0)
		{
			// Color texture attached to FBO is deleted
			RLGL::rlUnloadTexture(target.texture.id);
			RLGL::rlUnloadTexture(target.depth.id);

			// NOTE: Depth texture is automatically
			// queried and deleted before deleting framebuffer
			RLGL::rlUnloadFramebuffer(target.id);
		}
	}

	struct cameraData
	{
	public:
		float nearPlane;
		float farPlane;

		cameraData(float nP, float fP)
		{
			this->nearPlane = nP;
			this->farPlane = fP;
		}
	};

	void unloadCameraData(cameraData* data)
	{
		delete data;
	}

	public ref class ScriptableRenderPipeline abstract
	{
	protected:
		int height, width;

	public:
		Engine::Native::EnginePtr<RAYLIB::RenderTexture>* framebufferTexturePtr;
		Engine::Native::EnginePtr<RAYLIB::RenderTexture>* depthbufferTexturePtr;
		Engine::Native::EnginePtr<RAYLIB::Shader>* depthShaderPtr;
		Engine::Native::EnginePtr<cameraData*>* cameraDataPtr;

	public:
		ScriptableRenderPipeline()
		{
			Logging::Log("Creating new ScriptableRenderPipeline");

			Singleton<ScriptableRenderPipeline^>::Create(this);
			Singleton<HarmonyLib::Harmony^>::Instance->UnpatchAll("HarmonyInstance");

			this->height = Screen::Height;
			this->width = Screen::Width;

			RAYLIB::RenderTexture renderTexture = LoadRenderTextureDepthTex(Screen::Width, Screen::Height);
			framebufferTexturePtr = new Engine::Native::EnginePtr<RAYLIB::RenderTexture>(renderTexture, &UnloadRenderTextureDepthTex);

			RAYLIB::Shader depthShader = RAYLIB::LoadShader("Data/Engine/Shaders/base.vs", "Data/Engine/Shaders/depth.frag");
			depthShaderPtr = new Engine::Native::EnginePtr<RAYLIB::Shader>(depthShader, &RAYLIB::UnloadShader);

			RAYLIB::RenderTexture depthTexture = RAYLIB::LoadRenderTexture(Screen::Width, Screen::Height);
			depthbufferTexturePtr = new Engine::Native::EnginePtr<RAYLIB::RenderTexture>(depthTexture, &RAYLIB::UnloadRenderTexture);
		}

	protected:
		void CreateTexture()
		{
			if ((this->height != Screen::Height) || (this->width != Screen::Width))
			{
				if (framebufferTexturePtr != nullptr)
					delete framebufferTexturePtr;

				if (depthbufferTexturePtr != nullptr)
					delete depthbufferTexturePtr;

				RAYLIB::RenderTexture renderTexture = LoadRenderTextureDepthTex(Screen::Width, Screen::Height);
				framebufferTexturePtr = new Engine::Native::EnginePtr<RAYLIB::RenderTexture>(renderTexture, &UnloadRenderTextureDepthTex);

				RAYLIB::RenderTexture depthTexture = RAYLIB::LoadRenderTexture(Screen::Width, Screen::Height);
				depthbufferTexturePtr = new Engine::Native::EnginePtr<RAYLIB::RenderTexture>(depthTexture, &RAYLIB::UnloadRenderTexture);

				this->width = Screen::Width;
				this->height = Screen::Height;
			}
		}

		RenderTexture2D LoadRenderTextureDepthTex(int width, int height)
		{
			RenderTexture2D target = { 0 };

			target.id = RLGL::rlLoadFramebuffer(); // Load an empty framebuffer

			if (target.id > 0)
			{
				RLGL::rlEnableFramebuffer(target.id);

				// Create color texture (default to RGBA)
				target.texture.id = RLGL::rlLoadTexture(0, width, height, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
				target.texture.width = width;
				target.texture.height = height;
				target.texture.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
				target.texture.mipmaps = 1;

				// Create depth texture buffer (instead of raylib default renderbuffer)
				target.depth.id = RLGL::rlLoadTextureDepth(width, height, false);
				target.depth.width = width;
				target.depth.height = height;
				target.depth.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;       //DEPTH_COMPONENT_24BIT?
				target.depth.mipmaps = 1;

				// Attach color texture and depth texture to FBO
				RLGL::rlFramebufferAttach(target.id, target.texture.id, RLGL::RL_ATTACHMENT_COLOR_CHANNEL0, RLGL::RL_ATTACHMENT_TEXTURE2D, 0);
				RLGL::rlFramebufferAttach(target.id, target.depth.id, RLGL::RL_ATTACHMENT_DEPTH, RLGL::RL_ATTACHMENT_TEXTURE2D, 0);

				// Check if fbo is complete with attachments (valid)
				if (RLGL::rlFramebufferComplete(target.id)) TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", target.id);

				RLGL::rlDisableFramebuffer();
			}
			else TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");

			return target;
		}

	public:
		void ExecuteRenderWorkflow(Engine::Window^ windowHandle, Engine::Management::Scene^ scene)
		{
			PreFirstPassRender(scene);
			PostFirstPassRender();

			BeginDrawing();
			{
				CreateTexture();

				BeginTextureMode(this->framebufferTexturePtr->getInstance());
	
				OnRenderBegin();

				ClearBackground(GetColor(scene->skyColor));
				RLGL::rlClearScreenBuffers();
				RLGL::rlEnableDepthTest();
				PreRenderFrame();
				{
					Engine::EngineObjects::Camera^ camera = ObjectManager::singleton()->GetMainCamera();

					int currentLayer = 0;

					if (camera == nullptr)
						goto RENDER_END;

					bool is3DCamera = camera->is3DCamera();

					if (cameraDataPtr != nullptr)
						delete cameraDataPtr;

					cameraDataPtr = new Engine::Native::EnginePtr<cameraData*>(new cameraData(camera->nearPlane, camera->farPlane), &unloadCameraData);

					auto camera3D = ((Engine::EngineObjects::Native::NativeCamera3D*)camera->get());

					BeginMode3D(camera3D->get());

					render(currentLayer, scene);

					EndMode3D();

				RENDER_END:

					renderGUI(currentLayer, scene);
				}
				PostRenderFrame();
				RLGL::rlDisableDepthTest();
				EndTextureMode();

				int nearPlaneLOC = GetShaderLocation(depthShaderPtr->getInstance(), "nearPlane");
				int farPlaneLOC = GetShaderLocation(depthShaderPtr->getInstance(), "farPlane");
				int depthTextureLOC = GetShaderLocation(depthShaderPtr->getInstance(), "texture0");

				float nearPlane = cameraDataPtr->getInstance()->nearPlane;
				float farPlane = cameraDataPtr->getInstance()->farPlane;

				SetShaderValue(depthShaderPtr->getInstance(), nearPlaneLOC, &nearPlane, RAYLIB::SHADER_UNIFORM_FLOAT);
				SetShaderValue(depthShaderPtr->getInstance(), farPlaneLOC, &farPlane, RAYLIB::SHADER_UNIFORM_FLOAT);
				SetShaderValue(depthShaderPtr->getInstance(), depthTextureLOC, &framebufferTexturePtr->getInstance().depth.id, RAYLIB::SHADER_UNIFORM_SAMPLER2D);

				BeginShaderMode(depthShaderPtr->getInstance());
				BeginTextureMode(depthbufferTexturePtr->getInstance());
				{
					Rectangle target;
					target.x = 0;
					target.y = 0;
					target.width = Screen::Width;
					target.height = -Screen::Height;

					ClearBackground({ 0,0,0,255 });

					DrawTextureRec(framebufferTexturePtr->getInstance().depth, target, { 0,0 }, { 255,255,255,255 });
				}
				EndTextureMode();
				EndShaderMode();

				Rectangle target;
				target.x = 0;
				target.y = 0;
				target.width = Screen::Width;
				target.height = -Screen::Height;

				ClearBackground({ 0,0,0,255 });

				DrawTextureRec(depthbufferTexturePtr->getInstance().texture, target, { 0,0 }, { 255,255,255,255 });

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
		}

		void ExecuteRenderWorkflow_Editor(Engine::Window^ windowHandle, Engine::Management::Scene^ scene)
		{
			PreFirstPassRender(scene); // FIRSTPASS
			PostFirstPassRender();

			BeginDrawing();
			{
				CreateTexture();

				BeginTextureMode(this->framebufferTexturePtr->getInstance());

				OnRenderBegin(); // BEGIN

				ClearBackground(GetColor(scene->skyColor));

				PreRenderFrame(); // PRE FRAME
				{
					Engine::EngineObjects::Camera^ camera = ObjectManager::singleton()->GetMainCamera();

					int currentLayer = 1;

					if (camera == nullptr)
						goto RENDER_END;

					bool is3DCamera = camera->is3DCamera();

					BeginMode3D(((Engine::EngineObjects::Native::NativeCamera3D*)camera->get())->get());

					render(currentLayer, scene);

					EndMode3D();

				RENDER_END:

					renderGUI(currentLayer, scene);
				}

				EndTextureMode();

				OnRenderEnd();

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
		void renderGUI(int currentLayer, Engine::Management::Scene^ scene)
		{
			Layer^ lastLayer = nullptr;

			while (currentLayer != LayerManager::getHigherLayer())
			{
				Layer^ cL = LayerManager::GetLayerFromId(currentLayer);

				if (cL != nullptr)
				{
					for each (Engine::Management::MiddleLevel::SceneObject ^ sceneObject in scene->GetRenderQueue())
					{
						if (scene->sceneLoaded())
						{
							Engine::Internal::Components::GameObject^ reference = (Engine::Internal::Components::GameObject^)sceneObject->GetReference();

							if (reference->layerMask->IsLayer(cL))
							{
								if (reference->active)
									reference->DrawGUI();
							}
						}
					}

					Layer^ nextLayer = LayerManager::getNextHigherLayer(cL);

					if (nextLayer != nullptr)
						currentLayer = nextLayer->layerMask;
					else
						break;

					lastLayer = cL;
				}
				else
				{
					if (lastLayer == nullptr)
						break;

					Layer^ nextLayer = LayerManager::getNextHigherLayer(lastLayer);

					if (nextLayer != nullptr)
						currentLayer = nextLayer->layerMask;
					else
						break;
				}

			}
		}

	protected:
		void render(int currentLayer, Engine::Management::Scene^ scene)
		{
			if (currentLayer == LayerManager::getHigherLayer())
				return;
			else
			{
				//ObjectManager::singleton()->GetFirstObjectOfType<Engine::EngineObjects::LightManager^>()->LightUpdate();

				PreRenderObjects();

				Layer^ cL = LayerManager::GetLayerFromId(currentLayer);

				if (cL != nullptr)
				{
					for each (Engine::Management::MiddleLevel::SceneObject ^ sceneObject in scene->GetRenderQueue())
					{
						if (scene->sceneLoaded())
						{
							Engine::Internal::Components::GameObject^ reference = (Engine::Internal::Components::GameObject^)sceneObject->GetReference();

							if (reference->layerMask->IsLayer(cL))
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
						return render(nextLayer->layerMask, scene);
					else
						return;
				}
				else
					return render(currentLayer++, scene);

				PostRenderObjects();
			}

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
					continue;

				for each (Engine::Management::MiddleLevel::SceneObject ^ sceneObject in scene->GetRenderQueue())
				{
					if (scene->sceneLoaded())
					{
						Engine::Internal::Components::GameObject^ reference = (Engine::Internal::Components::GameObject^)sceneObject->GetReference();

						if (reference->layerMask->IsLayer(cL))
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

		void firstPassRender(int currentLayer, System::Collections::Generic::List< Engine::Internal::Components::GameObject^>^% objects)
		{
			if (Engine::Scripting::ObjectManager::singleton() == nullptr || !Singleton<Engine::Management::Scene^>::Instantiated)
				return;

			Engine::Management::Scene^ scene = Singleton<Engine::Management::Scene^>::Instance;

			while (currentLayer != LayerManager::getHigherLayer())
			{
				Layer^ cL = LayerManager::GetLayerFromId(currentLayer);

				if (cL == nullptr)
					continue;

				for each (Engine::Management::MiddleLevel::SceneObject ^ sceneObject in scene->GetRenderQueue())
				{
					if (scene->sceneLoaded())
					{
						Engine::Internal::Components::GameObject^ reference = (Engine::Internal::Components::GameObject^)sceneObject->GetReference();

						if (reference->layerMask->IsLayer(cL))
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
		virtual void PreRenderObject(Engine::Internal::Components::GameObject^) abstract;
		virtual void PostRenderObject() abstract;

		// Hooks at the beggining and end of the render process
		virtual void OnRenderBegin() abstract;
		virtual void OnRenderEnd() abstract;

		// FirstPass Rendering
		virtual void PreFirstPassRender(Engine::Management::Scene^) abstract;
		virtual void PostFirstPassRender() abstract;

		// For freeing resources and memory
		virtual void OnUnloadPipeline() { delete framebufferTexturePtr; delete depthbufferTexturePtr; delete depthShaderPtr; };
	};
}