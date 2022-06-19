
#include <memory>

#include "glad/gl.h"

#include "core/Engine.h"

#include "testScenes/TestSelector.h"

#include "scene/components/BasicComponents.h"
#include "scene/components/DrawableComponents.h"

using namespace YingLong;

int main()
{
	std::shared_ptr<Engine> engine = std::make_shared<Engine>("Test YingLong");

	Scene_SPtr TestSelector = std::make_shared<TestSelectorScene>(engine);
	engine->AddScene(TestSelector);

	Renderer::SetUnit(RendererUnit::Centimeter);

	engine->MainLoop();
}