generic <class T>
public ref class Singleton
{
public:
	static T Instance;
	static bool Instantiated = false;

public:
	static void Create(T inst)
	{
		Instance = inst;
		Instantiated = true;
	}

public:
	static void Release()
	{
		Instance = {};
		Instantiated = false;
	}
};

[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
private ref class LuaSingletonProxy
{
public:
	generic <typename T>
	static T Instance()
	{
		return Singleton<T>::Instance;
	}

	generic <typename T>
	static bool Instantiated()
	{
		return Singleton<T>::Instantiated;
	}

	generic <typename T>
	static void Create(T instance)
	{
		return Singleton<T>::Create(instance);
	}
};

[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
public ref class SharedInstance 
{
public:
	System::Object^ Instance;

public:
	SharedInstance(System::Object^ inst)
	{
		Instance = inst;
	}

private:
	static System::Collections::Generic::Dictionary<System::String^, SharedInstance^>^ Instances = gcnew System::Collections::Generic::Dictionary<System::String^, SharedInstance^>();

public:
	static void Create(System::String^ name, System::Object^ i)
	{
		if (!Instances->ContainsKey(name))
		{

		}
		else
		{
			Instances[name]->Instance = i;
		}
		Instances->Add(name, gcnew SharedInstance(i));
	}

public:
	static System::Object^ Get(System::String^ name)
	{
		return Instances[name];
	}

public:
    void Release()
	{
		Instance = nullptr;
	}
};