#include "TestGame.h"
#include "Core/Utilities/Log.h"

#include "Mode.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Renderer/GLTFLoading/GLTFParser.h"

#ifdef TESTGAME
#include "EntryPoint.h"
Crown::Application* Crown::CreateApplication()
{
    return new TestGame();
}
#endif

void TestGame::OnUpdate(float a_DeltaTime)
{
    //LOG("Update TestGame");
}

void TestGame::OnRender()
{
    //LOG("Update TestGame");
	window.clear();
	window.display();
}

void TestGame::OnStart()
{
    /* //ResourceManager Test code
    std::string resourceName = "TestModel";

    std::shared_ptr<ExampleResource> model = std::make_shared<ExampleResource>();

    GetResourceManager()->Add(resourceName, model);
    LOG("model test: " + std::to_string(GetResourceManager()->Get<ExampleResource>(resourceName)->Test));
    std::to_string(GetResourceManager()->Get<ExampleResource>("nothing");
    */

	std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
	bool isFullscreen = false; //modes.size() > 0;
	window.create(isFullscreen ? modes.at(0) : sf::VideoMode(1920, 1080), "SFML Window", isFullscreen ? sf::Style::Fullscreen : sf::Style::Default);
	sf::View view = window.getView();
	view.setCenter(sf::Vector2f(0.0f, 0.0f));
	window.setView(view);
}

void TestGame::OnLateUpdate(float a_DeltaTime)
{
}

void TestGame::OnFixedUpdate()
{
}

void TestGame::OnImGuiRender()
{
}
