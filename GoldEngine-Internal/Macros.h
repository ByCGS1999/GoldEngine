#pragma once

#pragma region Coding_Macros

// Coding Macros

#define Serialize Newtonsoft::Json::JsonConvert::SerializeObject
#define Deserialize Newtonsoft::Json::JsonConvert::DeserializeObject
#define JSON_SERIALIZE Newtonsoft::Json::JsonPropertyAttribute
#define DllExport __declspec(dllexport)
#define DllImport __declspec(dllimport)
#define Engine_GCObject Engine::Internal::Components::Object^

#pragma endregion

#pragma region Preprocessor_Definitions

// Preprocessor defines

#define LOGAPI_IMPL
#define RLIGHTS_IMPLEMENTATION

#define RLGL_SHOW_GL_DETAILS_INFO
#define RLGL_ENABLE_OPENGL_DEBUG_CONTEXT
#define GRAPHICS_API_OPENGL_33

#pragma endregion

#pragma region Engine_Configuration

// ENGINE CONFIGURATION

#define _CRT_SECURE_NO_WARNINGS
#define ENCRYPTION_PASSWORD "ChaosLanguage"

#define HIDE_CONSOLE false

#define MAX_LIGHTS 4

#define ENGINE_VERSION "GoldEngine Ver: 0.3c"
#define EDITOR_VERSION "GoldEngine Editor Ver: 0.6a"
#define RUNTIME_VERSION "GoldEngine Runtime Prev-Ver: 0.1a"

#pragma endregion