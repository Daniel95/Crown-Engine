#include "CrownPCH.h"
#include "PCRenderer.h"
#include "Shader.h"

#include "Core/Renderer/VertexArray.h"
#include "Core/Renderer/Models.h"
#include "Core/Renderer/Texture.h"
#include "Core/ECS/Component/RenderComponentSystem.h"
#include "Core/Renderer/ParticleSystem.h"
#include "PCParticleRenderData.h"
#include "Core/Renderer/AnimatedModel.h"
#include "Core/Time/Time.h"

namespace Crown
{
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
	
	PCRenderer::~PCRenderer()
	{
		CROWN_PROFILE_FUNCTION();

		delete m_Shaders;
		delete m_TestParticleSystem;
		delete m_TestAnimatedModel;
	}

	void PCRenderer::Init()
	{
		CROWN_PROFILE_FUNCTION();
		glViewport(0, 0, m_ViewWidth, m_ViewHeight);
		glEnable(GL_DEPTH_TEST);
		
		m_Shaders = new Shader("../Engine/assets/shaders/GLSL/VertexShader.vert", "../Engine/assets/shaders/GLSL/FragmentShader.frag");

		//TRANSFORM MATRICES 
		m_CameraPos =	glm::vec3(0.0f, 0.0f, 3.0f);
		m_CamDir =		glm::vec3(0.0f, 0.0f, -1.0f);
		m_CamTarget =	glm::vec3(0.0f, 0.0f, -1.0f);
		m_CamUp =		glm::vec3(0.0f, 1.0f, 0.0f);

		m_TestParticleSystem = new ParticleSystem();
		m_TestAnimatedModel = new AnimatedModel();
		m_TestAnimatedModel->Load("../Engine/assets/models/Fox/glTF/", "Fox.gltf");
		std::vector<std::string> animationNames = m_TestAnimatedModel->GetAllAnimationNames(0);
		m_TestAnimatedModel->SetAnimationByName(0, animationNames[2]);
	}

	void PCRenderer::BeginFrame(const CameraComponent& a_CameraComponent)
	{
		CROWN_PROFILE_FUNCTION();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(m_Wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		for (auto& renderComponent : m_RenderComponents)
		{
			renderComponent.model->vertexArray->Bind();
			glActiveTexture(GL_TEXTURE0);
			renderComponent.model->matData->textureData.front()->Bind();
			renderComponent.model->vertexArray->GetVertexBuffers().at(0)->Bind();
			renderComponent.model->vertexArray->GetIndexBuffer()->Bind();
			m_Shaders->Use();

			glm::mat4 mvpMat = a_CameraComponent.viewProjectionMatrix * renderComponent.modelMatrix;
			m_Shaders->setMat4("mvp", mvpMat);
			m_Shaders->setVec4("tint", renderComponent.model->matData->color);

			glDrawElements(GL_TRIANGLES, renderComponent.model->vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
		}

		m_RenderComponents.clear();
		m_ModelMatrices.clear();

		float currentTime = Time::GetTime();
		float deltaTime = currentTime - m_PreviousFrameTime;
		m_PreviousFrameTime = Time::GetTime();
		
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, -1.0f, -3.5f));
		if (m_TestAnimatedModel != nullptr)
		{
			m_TestAnimatedModel->Update(deltaTime);

			glm::mat4 animModelMat = translation;
			animModelMat = glm::scale(animModelMat, glm::vec3(0.0125f));
			m_TestAnimatedModel->Draw(a_CameraComponent.viewProjectionMatrix * animModelMat, animModelMat);
		}
		if(m_TestParticleSystem != nullptr)
		{
			m_TestParticleSystem->Update(deltaTime);

			PCParticleRenderData& particleRenderData = static_cast<PCParticleRenderData&>(m_TestParticleSystem->GetParticleRenderData());

			glm::mat4 particleModelMat = translation;
			particleModelMat = glm::scale(particleModelMat, glm::vec3(0.3f));
			particleRenderData.Draw(a_CameraComponent.viewProjectionMatrix * particleModelMat, glm::inverse(a_CameraComponent.viewMatrix));
		}
		
	}

	void PCRenderer::EndFrame()
	{
		
	}

	void PCRenderer::Render()
	{
		
	}

	RenderAPI PCRenderer::GetRenderAPI()
	{
		return RenderAPI::OpenGL;
	}

	void PCRenderer::Submit(const RenderComponent& a_RenderComponent)
	{
		CROWN_PROFILE_FUNCTION();

		m_RenderComponents.push_back(a_RenderComponent);
	}
}
