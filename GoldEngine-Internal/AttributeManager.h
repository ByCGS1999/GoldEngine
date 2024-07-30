#pragma once

namespace Engine::Scripting
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class AttributeManager
	{
	public:
		List<Attribute^>^ attributes;

	public:
		AttributeManager()
		{
			attributes = gcnew List<Attribute^>();
		}

		bool hasAttribute(String^ attribute)
		{
			for each (Attribute ^ attrib in attributes)
			{
				if (attrib != nullptr && attrib->name == attribute)
					return true;
			}

			return false;
		}

	private:
		Attribute^ GetAttribute(String^ attribute)
		{
			for each (Attribute ^ attrib in attributes)
			{
				if (attrib != nullptr && attrib->name == attribute)
					return attrib;
			}

			return nullptr;
		}


	public:
		void DeserializeAttributes()
		{
			for each (auto x in attributes)
			{
				printConsole("Deserializing Attribute -> " + x->name);
				x->setValue(x->DeserializeAttribute(), true);
			}
		}
		
		void removeAttribute(Attribute^ attrib)
		{
			attributes->Remove(attrib);
		}

		void removeAttribute(String^ name)
		{
			if (hasAttribute(name))
				attributes->Remove(GetAttribute(name));
		}

		void clearAttributes()
		{
			attributes->Clear();
		}

		void addAttribute(Attribute^ attribute) 
		{
			setAttribute(attribute);
		}

		void addAttribute(String^ name, System::Object^ data)
		{
			setAttribute(name, data);
		}

		void setAttribute(String^ name, System::Object^ data)
		{
			if (!hasAttribute(name))
			{
				attributes->Add(Attribute::New(name, data));
			}
			else
			{
				printError("Attribute name is duplicate");
			}
		}

		void setAttribute(Attribute^ newAttrib)
		{
			if (newAttrib == nullptr)
				return;


			if (!hasAttribute(newAttrib->name))
			{
				attributes->Add(newAttrib);
			}
			else
			{
				printError("Attribute name is duplicate");
			}
		}

		Attribute^ getAttribute(String^ name)
		{
			if (hasAttribute(name))
			{
				return GetAttribute(name);
			}
			else
			{
				return nullptr;
			}
		}
	};
}