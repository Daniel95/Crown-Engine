#pragma once
#include "Core/Application.h"
#include <SFML/Graphics.hpp>

class TestGame : public Crown::Application
{
private:
	sf::RenderWindow window;
protected:
    void OnUpdate(float a_DeltaTime) override;
    void OnRender() override;
    void OnStart() override;
    void OnLateUpdate(float a_DeltaTime) override;
    void OnFixedUpdate() override;
    void OnImGuiRender() override;
};
