#pragma once
using namespace System;
using namespace System::IO;
using namespace System::IO::Compression;

public ref class FileManager
{
	static String^ fileHeader = "GOLD ENGINE";
	static short int fileVersion = 100;
public:
	static void WriteCustomFileFormat(String^ fileName, int passwd)
	{
		ZipArchive^ archive = gcnew ZipArchive(File::Open(fileName, FileMode::OpenOrCreate, FileAccess::ReadWrite, FileShare::ReadWrite), ZipArchiveMode::Create | ZipArchiveMode::Update);
		
		archive->CreateEntry(fileHeader);
		archive->CreateEntry(fileVersion.ToString());
		archive->CreateEntry("Skybox_cube.glb");

		archive->CreateEntry(File::ReadAllText("Data/Engine/Models/Skybox_cube.glb"));
		
	}

	static void ReadCustomFileFormat(String^ fileName, int passwd)
	{
		BinaryReader^ reader = gcnew BinaryReader(File::Open(fileName, FileMode::OpenOrCreate, FileAccess::ReadWrite, FileShare::ReadWrite));

		Console::WriteLine(reader->ReadString());
		Console::WriteLine(reader->ReadInt16());

		auto dirInfo = Directory::CreateDirectory("Data/tmp/");
		WinAPI::SetAttribute("Data/tmp", 2);
		auto x = File::CreateText("Data/tmp/" + reader->ReadString());
		x->Write(reader->ReadString());
		x->Close();
		reader->Close();

	}
};

