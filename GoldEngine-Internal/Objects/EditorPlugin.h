#pragma once

#if !defined(PRODUCTION_BUILD)

namespace Engine::EngineObjects::Editor
{
	public interface class IEditorPlugin
	{
	public:
		virtual void OnEnablePlugin();
		virtual void PluginDraw();
		virtual void PluginUpdate();
		virtual void PluginDrawImGUI();
		virtual void OnDisablePlugin();
	};
}

#endif