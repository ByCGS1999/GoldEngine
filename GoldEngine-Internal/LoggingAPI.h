#pragma once

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;

#define printConsole Engine::Scripting::Logging::Log
#define printError Engine::Scripting::Logging::LogError
#define printWarning Engine::Scripting::Logging::LogWarning
#define printDebug Engine::Scripting::Logging::LogDebug

namespace Engine::Scripting
{
	public ref class Log
	{
	public:
		TraceLogLevel logType;
		String^ message;

		Log(TraceLogLevel level, String^ mesg)
		{
			logType = level;
			message = mesg;
		}
	};

	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class Logging abstract
	{
	private:
		static List<Log^>^ log = gcnew System::Collections::Generic::List<Engine::Scripting::Log^>();

	public:
		static void Log(String^ message)
		{
			TraceLog(LOG_INFO, CastStringToNative(message).c_str());
			log->Add(gcnew Engine::Scripting::Log(LOG_INFO, "[INFO] " + message));
		}

		static void LogDebug(String^ message)
		{
			TraceLog(LOG_DEBUG, CastStringToNative(message).c_str());
			log->Add(gcnew Engine::Scripting::Log(LOG_DEBUG, "[DEBUG] " + message));
		}

		static void LogWarning(String^ message)
		{
			TraceLog(LOG_WARNING, CastStringToNative(message).c_str());
			log->Add(gcnew Engine::Scripting::Log(LOG_WARNING, "[WARNING] " + message));
		}

		static void LogFatal(String^ message)
		{
			log->Add(gcnew Engine::Scripting::Log(LOG_FATAL, "[FATAL] " + message));
			TraceLog(LOG_FATAL, CastStringToNative(message).c_str());
		}

		static void LogError(String^ message)
		{
			TraceLog(LOG_ERROR, CastStringToNative(message).c_str());
			log->Add(gcnew Engine::Scripting::Log(LOG_ERROR, "[ERROR] " + message));
		}

		static void clearLogs()
		{
#if PRODUCTION_BUILD
			printWarning("Cannot use clearLogs on a production build (game).");
#else
			log->Clear();
#endif
		}

		static auto getLogs() 
		{
			return log->ToArray();
		}
	};
}