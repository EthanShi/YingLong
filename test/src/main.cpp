
#include <memory>

#include "core/Engine.h"

using namespace YingLong;

int main()
{
	std::shared_ptr<Engine> engine(new Engine("Test YingLong"));
	engine->MainLoop();
}