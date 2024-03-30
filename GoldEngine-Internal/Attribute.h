
namespace Engine::Scripting
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class Attribute
	{
	public:
		String^ name;
		System::Object^ userData;
		System::String^ type;

	public:
		Attribute(String^ str, System::Object^ data, String^ dataType)
		{
			this->name = str;
			userData = data;
			type = dataType;
		}

	public:
		static Attribute^ New(String^ name, System::Object^ data)
		{
			return gcnew Attribute(name, data, data->GetType()->Name);
		}

	};
}