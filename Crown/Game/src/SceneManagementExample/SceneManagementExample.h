#pragma once
#include "Core/Application.h"

#include <memory>
#include <vector>
#include "Core/ECS/Entity/Entity.h"
#include "Core/SceneManagement/Scene.h"

class SceneManagementExample : public Crown::Application
{
protected:
	void OnUpdate(float a_DeltaTime) override;
	void OnRender() override;
	void OnStart() override;
	void OnLateUpdate(float a_DeltaTime) override;
	void OnFixedUpdate() override;
	void OnImGuiRender() override;

private:
	std::vector<Crown::Entity> m_Entities;
};
