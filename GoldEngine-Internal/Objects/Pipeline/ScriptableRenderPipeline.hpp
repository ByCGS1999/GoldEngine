public ref class ScriptableRenderPipeline abstract
{
public:
	ScriptableRenderPipeline()
	{
		Singleton<ScriptableRenderPipeline^>::Create(this);
	}
	
public:
	virtual void PreRenderFrame() abstract;

	virtual void PreRenderObject() abstract;

	virtual void PostRenderObject() abstract;

	virtual void PostRenderFrame() abstract;
};