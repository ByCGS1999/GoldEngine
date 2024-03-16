#pragma once

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;

namespace Engine::Scripting
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class Logging abstract
	{
	public:
		static void Log(String^ message)
		{
			TraceLog(LOG_INFO, CastStringToNative(message).c_str());
		}

		static void LogWarning(String^ message)
		{
			TraceLog(LOG_WARNING, CastStringToNative(message).c_str());
		}

		static void LogFatal(String^ message)
		{
			TraceLog(LOG_FATAL, CastStringToNative(message).c_str());
		}

		static void LogError(String^ message)
		{
			TraceLog(LOG_ERROR, CastStringToNative(message).c_str());
		}
	};
}