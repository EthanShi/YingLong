
#include "core/Engine.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

using namespace YingLong;

int main()
{
	ImGui::CreateContext();
	std::shared_ptr<Engine> engine(new Engine("Test YingLong"));
	engine->MainLoop();
}