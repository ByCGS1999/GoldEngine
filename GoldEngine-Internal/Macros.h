#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define ENCRYPTION_PASSWORD "ChaosLanguage"

#define RLIGHTS_IMPLEMENTATION
#define MAX_LIGHTS max_lights
#define ToJSON Newtonsoft::Json::JsonConvert::SerializeObject
#define ToObject Newtonsoft::Json::JsonConvert::DeserializeObject
#define DllExport __declspec(dllexport)
#define DllImport __declspec(dllimport)
#define Engine_GCObject Engine::Internal::Components::Object^
#define PRODUCTION_BUILD false
#define print printf

#define ENGINE_VERSION "GoldEngine Ver: 0.3c"
#define EDITOR_VERSION "GoldEngine Editor Ver: 0.6a"
#define RUNTIME_VERSION "GoldEngine Runtime Prev-Ver: 0.1a"

const int max_lights = 4;
unsigned int passwd = 0;