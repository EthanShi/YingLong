#pragma once

#include <string>

#include "GLFW/glfw3.h"
#include "entt/entt.hpp"

#include "Macros.h"

namespace YingLong {

	class YINGLONG_API Engine
	{
	public:
		Engine(const std::string& WindowTitle);
		~Engine();

		void MainLoop();

	public:

	private:
		static void OnFrameSizeChanged(GLFWwindow* Window, int32 Width, int32 Height);
	
	private:
		GLFWwindow* m_Window;
		const int32 m_DEFAULT_WINDOW_WIDTH = 1280;
		const int32 m_DEFAULT_WINDOW_HEIGHT = 720;

		std::string m_WindowTitle = "YingLong";

		entt::registry m_Registry;

		uint64 m_LastFrameTime;
	};

}
