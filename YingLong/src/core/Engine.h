#pragma once

#include <string>

#include "GLFW/glfw3.h"

#include "core/Macros.h"
#include "scene/Scene.h"

namespace YingLong {

	class YINGLONG_API Engine : public std::enable_shared_from_this<Engine>
	{
	public:
		Engine(const std::string& WindowTitle);
		~Engine();

		void MainLoop();

		void AddScene(std::shared_ptr<Scene> scene);
		void RemoveScene(const std::shared_ptr<Scene>& scene);

	private:
		static void OnFrameSizeChanged(GLFWwindow* Window, int32 Width, int32 Height);
	
	private:
		GLFWwindow* m_Window = nullptr;
		const int32 m_DEFAULT_WINDOW_WIDTH = 1280;
		const int32 m_DEFAULT_WINDOW_HEIGHT = 720;

		std::string m_WindowTitle = "YingLong";

		std::vector<std::shared_ptr<Scene>> m_Scenes;

		uint64 m_LastFrameTime = 0;
	};

}
