#pragma once
using namespace System;
using namespace System::IO;

public ref class FileManager
{
	static String^ fileHeader = "GOLD ";
	static short int fileVersion = 100;

public:
	static void WriteToCustomFile(String^ fileName, String^ password, array<String^>^ inFile)
	{
		auto file = File::Open(fileName, FileMode::OpenOrCreate);

		auto stream = gcnew BinaryWriter(file);
		stream->Flush();
		stream->Write(fileHeader);
		stream->Write(fileVersion);

		auto deflateStream = gcnew Compression::DeflateStream(file, Compression::CompressionMode::Compress);
		stream = gcnew BinaryWriter(deflateStream);

		stream->Write(inFile->Length); // assets in file
		for (int x = 0; x < inFile->Length; x++)
		{
			auto fileName = inFile[x];
			stream->Write(fileName); // write model name
			auto contents = File::ReadAllBytes("Data/" + inFile[x]);
			stream->Write(contents->Length);
			stream->Write(contents);
		}

		stream->Close();
		deflateStream->Close();
		file->Close();
	}

	static void ReadCustomFileFormat(String^ fileName, String^ password)
	{
		auto file = File::Open(fileName, FileMode::OpenOrCreate);
		
		auto stream = gcnew BinaryReader(file);

		String^ header = stream->ReadString();

		Console::WriteLine(header);

		if (fileHeader->Equals(header))
		{
			short int version = stream->ReadInt16();

			Console::WriteLine(version);

			if (fileVersion == version)
			{
				auto deflateStream = gcnew Compression::DeflateStream(file, Compression::CompressionMode::Decompress);
				stream = gcnew BinaryReader(deflateStream);

				Directory::CreateDirectory("Data/tmp/");
				WinAPI::SetAttribute("Data/tmp/", 1);
				int assets = stream->ReadInt32();

				Console::WriteLine(assets);

				for (int x = 0; x < assets; x++)
				{
					String^ fN = stream->ReadString();
					unsigned long length = stream->ReadInt32();
					auto fC = stream->ReadBytes(length);
					Console::WriteLine(fN);
					Directory::CreateDirectory(Path::GetDirectoryName("Data/tmp/" + fN));
					auto fS = File::Open("Data/tmp/" + fN, FileMode::OpenOrCreate);
					auto bW = gcnew BinaryWriter(fS);
					bW->Flush();
					bW->Write(
						fC
					);
					bW->Close();
				}
			}
		}


		stream->Close();
	}

};

