#include "pch.h"
#include "CppUnitTest.h"
#include "Core/Resource/ResourceManager.h"
#include "Example/ExampleResource.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		
		TEST_METHOD(TestResourceManager)
		{
			std::shared_ptr<Crown::ResourceManager> resourceManager = std::make_shared<Crown::ResourceManager>();

            const std::shared_ptr<ExampleResource> model = ExampleResource::Create();

			resourceManager->Add("TestModel", model);

            const std::shared_ptr<ExampleResource> resourceModel = resourceManager->Get<ExampleResource>("TestModel");

			Assert::IsNotNull(resourceModel.get());
		}
	};
}
