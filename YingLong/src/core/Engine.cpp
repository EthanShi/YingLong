
#include "YingLongPCH.h"

#include "Engine.h"

#include "renderer/Camera.h"
#include "renderer/Renderer.h"
#include "input/Input.h"

namespace YingLong
{

	Engine::Engine(const std::string& WindowTitle)
		: m_WindowTitle(WindowTitle)
	{
		GLFWwindow* window;

		/* Initialize the library */
		assert(glfwInit());

		/* Create a windowed mode window and its OpenGL context */
		window = glfwCreateWindow(m_DEFAULT_WINDOW_WIDTH, m_DEFAULT_WINDOW_HEIGHT, m_WindowTitle.c_str(), NULL, NULL);
		if (!window)
		{
			glfwTerminate();
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(window);

		/* Set frame size callback */
		glfwSetFramebufferSizeCallback(window, &Engine::OnFrameSizeChanged);
		OnFrameSizeChanged(window, m_DEFAULT_WINDOW_WIDTH, m_DEFAULT_WINDOW_HEIGHT);

		glfwSwapInterval(5);

		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			std::cout << "Error: %s\n" << glewGetErrorString(err) << std::endl;
		}
		else
		{
			std::cout << glGetString(GL_VERSION) << std::endl;
		}

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 130");
		ImGui::StyleColorsDark();

		//test::Test* currentTest = nullptr;
		//test::TestMenu* testMenu = new test::TestMenu(currentTest);
		//RegisterTests(testMenu);
		//currentTest = testMenu;

		// Set default camera
		std::shared_ptr<Camera> camera = std::make_shared<Camera>();
		camera->SetPostion(glm::vec3(0.0f, 0.0f, 10.0f));
		Renderer::SetCamera(camera);
		Renderer::SetUnit(RendererUnit::Centimeter);
		Renderer::SetDepthTestEnable(true);

		// Set Input
		Input::InitInput(window);

		m_LastFrameTime = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
			).count();
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
			float deltaTime = (currentTime - m_LastFrameTime) * 0.001f;
			m_LastFrameTime = currentTime;

			Renderer::Clear();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			Input::ProcessInput();

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

	void Engine::OnFrameSizeChanged(GLFWwindow* Window, int32 Width, int32 Height)
	{
		glViewport(0, 0, Width, Height);
		Renderer::OnWindowSizeChanged(Width, Height);
	}

}