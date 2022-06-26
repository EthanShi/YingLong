#pragma once

#include <string>

#include "GLFW/glfw3.h"

#include "core/Macros.h"
#include "scene/Scene.h"
#include "scene/SceneTypes.h"

namespace YingLong {

	class YINGLONG_API Engine
	{
	public:
		Engine(const std::string& WindowTitle);
		~Engine();

		void MainLoop();

		void AddScene(const Scene_SPtr& scene);
		void RemoveScene(const Scene_SPtr& scene);

	public:

	private:
		static void OnFrameSizeChanged(GLFWwindow* Window, int32 Width, int32 Height);
	
	private:
		GLFWwindow* m_Window = nullptr;
		const int32 m_DEFAULT_WINDOW_WIDTH = 1280;
		const int32 m_DEFAULT_WINDOW_HEIGHT = 720;

		std::string m_WindowTitle = "YingLong";

		std::vector<Scene_SPtr> m_Scenes;

		uint64 m_LastFrameTime = 0;
	};

}
