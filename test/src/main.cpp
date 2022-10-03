
#include "core/Engine.h"

#include "testScenes/TestSelector.h"

#include "scene/components/BasicComponents.h"
#include "scene/components/DrawableComponents.h"

using namespace YingLong;

int main()
{
	std::shared_ptr<Engine> engine = std::make_unique<Engine>("Test", "Test YingLong");

	engine->AddScene(std::make_unique<TestSelectorScene>());

	Renderer::SetUnit(RendererUnit::Centimeter);

	engine->MainLoop();
}