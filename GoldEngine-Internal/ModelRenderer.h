#pragma once

namespace Engine::EngineObjects
{
	namespace Native
	{
		class NativeModelRenderer
		{
		public:
			std::unique_ptr<Model> model;
			std::unique_ptr<Material> material;
			unsigned int color_hex;

			NativeModelRenderer(Model m, Material mat, unsigned int hex) : model(std::make_unique<Model>(m)),
				material(std::make_unique<Material>(mat)),
				color_hex(hex)
			{

			}
		};
	}

	public ref class ModelRenderer : public Engine::Internal::Components::Object
	{
	private:
		Native::NativeModelRenderer* nativeRenderer;

	public:
		ModelRenderer(String^ name, Engine::Internal::Components::Transform^ trans);
		void SetNativeRenderer(Native::NativeModelRenderer* renderer);
		void Update() override;
		void PhysicsUpdate() override;
		void Start() override;
		void Draw() override;
		void SetColorTint(unsigned int hexValue);
	};
}