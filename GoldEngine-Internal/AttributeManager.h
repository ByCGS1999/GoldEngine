
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

	private:
		bool hasAttribute(String^ attribute) 
		{
			for each (Attribute^ attrib in attributes)
			{
				if (attrib->name == attribute)
					return true;
			}

			return false;
		}

		Attribute^ GetAttribute(String^ attribute)
		{
			for each (Attribute ^ attrib in attributes)
			{
				if (attrib->name == attribute)
					return attrib;
			}

			return nullptr;
		}


	public:
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