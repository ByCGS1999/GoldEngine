#pragma once

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;

#define printConsole Engine::Scripting::Logging::Log
#define printError Engine::Scripting::Logging::LogError
#define printWarning Engine::Scripting::Logging::LogWarning
#define printDebug Engine::Scripting::Logging::LogDebug
#define print Engine::Scripting::Logging::LogCustom

namespace Engine::Scripting
{
	public ref class Log
	{
	public:
		int logType;
		String^ message;

		Log(int level, String^ mesg)
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
			msclr::lock l(log);

			if (l.try_acquire(1000))
			{
				TraceLog(LOG_INFO, CastStringToNative(message).c_str());
				log->Add(gcnew Engine::Scripting::Log(LOG_INFO, "[INFO] " + message));
			}
			else
			{
				Log(message); // recurse call
			}

			l.release();
		}

		static void LogCustom(String^ header, String^ message)
		{
			msclr::lock l(log);

			if (l.try_acquire(1000))
			{
				TraceLog(LOG_INFO, CastStringToNative(message).c_str());
				log->Add(gcnew Engine::Scripting::Log(LOG_INFO, header + " " + message));
			}
			else
			{
				Log(message); // recurse call
			}

			l.release();
		}

		static void LogDebug(String^ message)
		{
			msclr::lock l(log);

			if (l.try_acquire(1000))
			{
				TraceLog(LOG_DEBUG, CastStringToNative(message).c_str());
				log->Add(gcnew Engine::Scripting::Log(LOG_DEBUG, "[DEBUG] " + message));
			}
			else
			{
				LogDebug(message); // recurse call
			}
			
			l.release();
		}

		static void LogWarning(String^ message)
		{
			msclr::lock l(log);

			if (l.try_acquire(1000))
			{
				TraceLog(LOG_WARNING, CastStringToNative(message).c_str());
				log->Add(gcnew Engine::Scripting::Log(LOG_WARNING, "[WARNING] " + message));
			}
			else
			{
				LogWarning(message); // recurse call
			}

			l.release();
		}

		static void LogFatal(String^ message)
		{
			msclr::lock l(log);

			if (l.try_acquire(1000))
			{
				log->Add(gcnew Engine::Scripting::Log(LOG_FATAL, "[FATAL] " + message));
				TraceLog(LOG_FATAL, CastStringToNative(message).c_str());
			}
			else
			{
				LogFatal(message); // recurse call
			}

			l.release();
		}

		static void LogError(String^ message)
		{
			msclr::lock l(log);

			if (l.try_acquire(1000))
			{
				TraceLog(LOG_ERROR, CastStringToNative(message).c_str());
				log->Add(gcnew Engine::Scripting::Log(LOG_ERROR, "[ERROR] " + message));
			}
			else
			{
				LogError(message); // recurse call
			}

			l.release();
		}

		static void clearLogs()
		{
			#if PRODUCTION_BUILD
				printWarning("Cannot use clearLogs on a production build (game).");
			#else
			msclr::lock l(log);

			if (l.try_acquire(1000))
				log->Clear();
			else
				clearLogs();

			l.release();

			#endif
		}

		static System::Collections::Generic::List<Engine::Scripting::Log^>^ getLogs()
		{
			if (log != nullptr)
				return log;
			else
				return nullptr;
		}
	};
}