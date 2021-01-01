#pragma once

	namespace tinygltf
	{
		struct Image;
	}
namespace Crown
{
	
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void Init() = 0;
		virtual void Load(const tinygltf::Image& a_MaterialData) = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static std::shared_ptr<Texture> Create();
	};
}