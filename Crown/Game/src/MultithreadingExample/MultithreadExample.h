#pragma once
#include "Core/Application.h"

#include <memory>
#include <vector>
#include "Core/ECS/Entity/Entity.h"

namespace Crown
{
    class CameraComponentSystem;
    class RenderComponentSystem;
    class TransformComponentSystem;
}

class MultithreadExample : public Crown::Application
{
protected:
    void OnUpdate(float a_DeltaTime) override;
    void OnRender() override;
    void OnStart() override;
    void OnLateUpdate(float a_DeltaTime) override;
    void OnFixedUpdate() override;
    void OnImGuiRender() override;

private:
    std::shared_ptr<Crown::CameraComponentSystem> m_CameraComponentSystem;

    std::vector<Crown::Entity> m_Entities;
    std::vector<int> m_NewV;
    float m_XRotation = 0;
    float m_YRotation = 0;
    float m_ZRotation = 0;
    float m_RotateSpeed = 0.3f;
    int m_ViewWidth = 800, m_ViewHeight = 600;
    float m_BaseMoveSpeed = 0.8f;
    float m_BaseRotateSpeed = 0.2f;

    float m_TempStoredDeltaTime;    //for FPS counter, temporary
    int m_TempStoredFrameCount;     //for FPS counter, temporary
};
