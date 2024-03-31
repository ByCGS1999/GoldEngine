namespace Engine::Utils
{
	using namespace System;
	using namespace System::IO;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace Engine::Scripting;

	ref class LogReporter
	{
	public:
		static LogReporter^ singleton;

	private:
		TextWriter^ fileStream;
		List<Log^>^ lifetimeLogs;
		Threading::Thread^ newThread;


	public:
		LogReporter(String^ fileName)
		{
			singleton = this;

			fileStream = File::CreateText(fileName);
			lifetimeLogs = gcnew System::Collections::Generic::List<Log^>();

			newThread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &LogReporter::ThreadProcessing));
			newThread->IsBackground = true;
			newThread->Start();
		}

	private:
		void WriteContentsToFile(String^ message)
		{
			fileStream->Write(message);
			fileStream->Write("\n");
		}

		void ThreadProcessing()
		{
			while (true)
			{
				for each (Log^ log in Logging::getLogs())
				{
					if (!lifetimeLogs->Contains(log))
					{
						WriteContentsToFile(log->message);
						lifetimeLogs->Add(log);
					}
				}
			}
		}

	public:
		void CloseThread()
		{
			if (newThread != nullptr)
				newThread->Abort();
		}

	};
}