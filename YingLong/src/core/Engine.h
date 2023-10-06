#pragma once

#include "YingLongPCH.h"

#include "Log.h"
#include "core/Macros.h"
#include "scene/Scene.h"

DECLARE_LOGGER(EngineLog)

namespace YingLong {

	class YINGLONG_API Engine : public std::enable_shared_from_this<Engine>
	{
	public:
		Engine(const std::string& ProjectName, const std::string& WindowTitle);
		~Engine();

		void MainLoop();

		void AddScene(std::shared_ptr<Scene> scene);
		void RemoveScene(const std::shared_ptr<Scene>& scene);

		const std::string& GetProjectName() { return ProjectName; }
		const std::string& GetWindowTitle() { return WindowTitle; }

	private:
		static void OnFrameSizeChanged(GLFWwindow* Window, int32 Width, int32 Height);
	
	private:
		GLFWwindow* Window = nullptr;
		const int32 DEFAULT_WINDOW_WIDTH = 1280;
		const int32 DEFAULT_WINDOW_HEIGHT = 720;

		std::string WindowTitle = "YingLong";
		std::string ProjectName = "YingLong";

		std::map<std::string, std::shared_ptr<Scene>> Scenes;

		uint64 LastFrameTime = 0;
	};

}
