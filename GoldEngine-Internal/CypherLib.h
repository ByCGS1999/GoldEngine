#pragma once

using namespace System;
using namespace System::Text;

ref class CypherLib
{
public:
	static unsigned int GetPasswordBytes(const char* passwd)
	{
		unsigned int retn = 0;

		char dest[sizeof(passwd)+1];

		strcpy(dest, passwd);

		for (char c : dest)
		{
			retn += (unsigned int)c;
		}

		return retn;
	}
public:
	static System::String^ EncryptFileContents(System::String^ fileContents, unsigned int password)
	{
		String^ encryptedContents = "";

		for each (char s in fileContents->ToCharArray())
		{
			int cId = (int)s;
			int byteResult = cId ^ password;
			encryptedContents += "/" + byteResult.ToString();
		}

		return encryptedContents->Substring(1, encryptedContents->Length-1);
	}

public:
	static System::String^ DecryptFileContents(System::String^ fileContents, unsigned int password)
	{
		System::String^ decryptedContents = "";

		auto decStr = fileContents->Split('/');

		for each (String ^ s in decStr)
		{
			decryptedContents += (Char)(System::Int32::Parse(s) ^ password);
		}

		return decryptedContents;
	}
};