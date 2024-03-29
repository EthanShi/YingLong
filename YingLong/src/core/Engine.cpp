
#include "YingLongPCH.h"

#include "Engine.h"

#include "renderer/Camera3D.h"
#include "renderer/Renderer3D.h"
#include "input/Input.h"
#include "Config.h"

DEFINE_LOGGER(EngineLog)

namespace YingLong
{

	Engine::Engine(const std::string& ProjectName, const std::string& WindowTitle)
		: WindowTitle(WindowTitle)
		, ProjectName(ProjectName)
	{
		// test config
		Config::Instance();

		// Set Log
		Log::Instance().SetLogFileName(ProjectName);

		/* Initialize the library */
		assert(glfwInit());

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

		EngineLog().info("Initialize GLFW library");

		/* Create a windowed mode window and its OpenGL context */
		Window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, WindowTitle.c_str(), NULL, NULL);
		if (!Window)
		{
			glfwTerminate();
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(Window);
		glfwSwapInterval(1);

		/* Set frame size callback */
		glfwSetFramebufferSizeCallback(Window, &Engine::OnFrameSizeChanged);

		int version = gladLoadGL(glfwGetProcAddress);
		if (version == 0) {
			EngineLog().error("Failed to initialize OpenGL context");
			return;
		}
		gladUninstallGLDebug();

		// Successfully loaded OpenGL
		EngineLog().info("Loaded OpenGL {}.{}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(Window, true);
		ImGui_ImplOpenGL3_Init("#version 130");

		// Set Input
		Input::Instance().InitInput(Window);

		LastFrameTime = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
			).count();

		OnFrameSizeChanged(Window, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

		EngineLog().info("Engine initialize finished.");
	}

	Engine::~Engine()
	{
		Scenes.clear();

		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(Window);
		glfwTerminate();
	}

	void Engine::MainLoop()
	{
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(Window))
		{
			/* Poll for and process events */
			glfwPollEvents();

			auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now().time_since_epoch()
				).count();
			float deltatime = (currentTime - LastFrameTime) * 0.001f;
			LastFrameTime = currentTime;

			Renderer::Clear();
			Renderer::DrawBackgroundColor();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			Input::Instance().ProcessInput();

			// Scenes may be change in scenes Tick, copy all scenes for this frame.
			std::map<std::string, std::shared_ptr<Scene>> CurScenes = Scenes;
			for (auto& scene : CurScenes)
			{
				scene.second->Tick(deltatime);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(Window);
		}
	}

	void Engine::AddScene(std::shared_ptr<Scene> scene)
	{
		auto FindResult = Scenes.find(scene->GetName());
		if (FindResult != Scenes.end())
		{
			// Log already added error
			return;
		}
		scene->OnActive(shared_from_this(), scene);
		Scenes[scene->GetName()] = std::move(scene);
	}

	void Engine::RemoveScene(const std::shared_ptr<Scene>& scene)
	{
		auto FindResult = Scenes.find(scene->GetName());
		if (FindResult == Scenes.end())
		{
			// Log can not find this scene error
			return;
		}
		scene->OnInactive();
		Scenes.erase(FindResult);
	}

	void Engine::OnFrameSizeChanged(GLFWwindow* Window, int32 Width, int32 Height)
	{
		glViewport(0, 0, Width, Height);
		Renderer::OnWindowSizeChanged(Width, Height);
	}

}