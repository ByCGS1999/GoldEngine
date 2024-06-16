#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define ENCRYPTION_PASSWORD "ChaosLanguage"

#define LOGAPI_IMPL
#define RLIGHTS_IMPLEMENTATION
#define MAX_LIGHTS 4
#define Serialize Newtonsoft::Json::JsonConvert::SerializeObject
#define Deserialize Newtonsoft::Json::JsonConvert::DeserializeObject

#define DllExport __declspec(dllexport)
#define DllImport __declspec(dllimport)
#define Engine_GCObject Engine::Internal::Components::Object^
#define PRODUCTION_BUILD false // if this is true the runtime will be compiled, otherwise, the editor will be compiled.

#define ENGINE_VERSION "GoldEngine Ver: 0.3c"
#define EDITOR_VERSION "GoldEngine Editor Ver: 0.6a"
#define RUNTIME_VERSION "GoldEngine Runtime Prev-Ver: 0.1a"

#define JSON_SERIALIZE Newtonsoft::Json::JsonPropertyAttribute