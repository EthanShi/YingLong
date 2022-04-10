
#include "YingLongPCH.h"

#include "Engine.h"

#include "renderer/Camera3D.h"
#include "renderer/Renderer3D.h"
#include "input/Input.h"

namespace YingLong
{

	Engine::Engine(const std::string& WindowTitle)
		: m_WindowTitle(WindowTitle)
	{
		/* Initialize the library */
		assert(glfwInit());

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

		/* Create a windowed mode window and its OpenGL context */
		m_Window = glfwCreateWindow(m_DEFAULT_WINDOW_WIDTH, m_DEFAULT_WINDOW_HEIGHT, m_WindowTitle.c_str(), NULL, NULL);
		if (!m_Window)
		{
			glfwTerminate();
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(5);

		/* Set frame size callback */
		glfwSetFramebufferSizeCallback(m_Window, &Engine::OnFrameSizeChanged);

		int version = gladLoadGL(glfwGetProcAddress);
		if (version == 0) {
			printf("Failed to initialize OpenGL context\n");
			return;
		}
		// Successfully loaded OpenGL
		printf("Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
		ImGui_ImplOpenGL3_Init("#version 130");

		// Set Input
		Input::InitInput(m_Window);

		m_LastFrameTime = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
			).count();

		OnFrameSizeChanged(m_Window, m_DEFAULT_WINDOW_WIDTH, m_DEFAULT_WINDOW_HEIGHT);
	}

	Engine::~Engine()
	{
		//if (currentTest != testMenu)
		//{
		//	delete testMenu;
		//}
		//delete currentTest;

		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void Engine::MainLoop()
	{
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(m_Window))
		{
			auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now().time_since_epoch()
				).count();
			float deltatime = (currentTime - m_LastFrameTime) * 0.001f;
			m_LastFrameTime = currentTime;

			Renderer::Clear();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			Input::ProcessInput();

			for (Scene_SPtr& scene : m_Scenes)
			{
				scene->Tick(deltatime);
			}

			//if (currentTest)
			//{
			//	currentTest->OnUpdate(deltaTime);
			//	currentTest->OnRender();

			//	ImGui::Begin("Tests");

			//	if (currentTest != testMenu && ImGui::Button("<-"))
			//	{
			//		delete currentTest;
			//		currentTest = testMenu;
			//	}

			//	currentTest->OnImguiRender();

			//	ImGui::End();
			//}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(m_Window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	void Engine::AddScene(const Scene_SPtr& scene)
	{
		m_Scenes.push_back(scene);
	}

	void Engine::RemoveScene(const Scene_SPtr& scene)
	{
		std::remove(m_Scenes.begin(), m_Scenes.end(), scene);
	}

	void Engine::OnFrameSizeChanged(GLFWwindow* Window, int32 Width, int32 Height)
	{
		glViewport(0, 0, Width, Height);
		Renderer::OnWindowSizeChanged(Width, Height);
	}

}