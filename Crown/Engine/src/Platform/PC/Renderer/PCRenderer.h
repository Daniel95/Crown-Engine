#pragma once
#include "Core/Renderer/Renderer.h"
#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>
#include <vector>
#include <memory>
#include "Core/ECS/Component/CameraComponentSystem.h"
#include "Core/ECS/Component/RenderComponentSystem.h"

#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Window.h"

struct GLFWwindow;
//struct DrawData;

namespace Crown
{
	class Shader;
	class ParticleSystem;
	class AnimatedModel;
	
	class PCRenderer : public Renderer
	{
	public:
		~PCRenderer() override;

		void Init()				override;
		void BeginFrame(const CameraComponent& a_CameraComponent) override;
		void EndFrame() override;
		void Render()			override;
		RenderAPI GetRenderAPI() override;
		
		void Submit(const RenderComponent& a_RenderComponent) override;
		
	private:

		glm::vec3 m_CamTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 m_CamDir;
		glm::vec3 m_CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 m_CamUp = glm::vec3(0.0f, 1.0f, 0.0f);	//initialized as worldspace up
		
		bool m_Wireframe = false;
		
		float m_DeltaTimeTemp = 0.0f;
		float m_LastFrameTemp = 0.0f;

		Shader* m_Shaders;

		int m_ViewWidth = 800, m_ViewHeight = 600;
		int m_Width, m_Height, m_NRChannels;
		unsigned char* m_Data;
		unsigned int m_TempTexture0;
		unsigned int m_TempTexture1;
		
		float m_VerticesArrCube[180] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		std::vector<glm::mat4> m_ModelMatrices;

		std::array<glm::vec3, 10> m_CubePositions = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		std::vector<float> quadVertices = {
			 0.5f,  0.5f, 0.0f,  // top right
			 0.5f, -0.5f, 0.0f,  // bottom right
			-0.5f, -0.5f, 0.0f,  // bottom left
			-0.5f,  0.5f, 0.0f   // top left 
		};

		std::vector<unsigned int> quadIndices = {
			0, 1, 3,  // first Triangle
			1, 2, 3   // second Triangle
		};

		///Should perhaps be migrated to another class in the future.
		//This holds all model data that has already been copied to GPU memory
		//std::vector<std::shared_ptr<ModelData>> m_CopiedModels;
		//std::vector<int> m_IndicesMarkedForCopy;

		std::vector<RenderComponent> m_RenderComponents;

		ParticleSystem* m_TestParticleSystem = nullptr;
		AnimatedModel* m_TestAnimatedModel = nullptr;
		float m_PreviousFrameTime = 0.f;
	};
}


