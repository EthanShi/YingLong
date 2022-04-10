
#include <memory>

#include "core/Engine.h"

#include "testScenes/clearColor.h"

using namespace YingLong;

int main()
{
	std::shared_ptr<Engine> engine(new Engine("Test YingLong"));

	Scene_SPtr TestClearColor = std::make_shared<ClearColorScene>();
	engine->AddScene(TestClearColor);

	engine->MainLoop();
}