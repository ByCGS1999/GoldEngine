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


const int max_lights = 4;