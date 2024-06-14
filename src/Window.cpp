#include <GL/glew.h>
#include "Core/Window.hpp"

namespace CR3Converter {
	GLFWwindow* Window::window = nullptr;
	int Window::width = 0;
	int Window::height = 0;
	bool Window::s_ShouldClose = false;

	std::optional<WindowError> Window::Init() {
		auto result = InitGLFW();
		if(result.has_value())
			return result;

		return std::nullopt;
	}

	std::optional<WindowError> Window::InitGLFW() {
		if(!glfwInit())
			return WindowError("Failed to initialize GLFW");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		Window::width = 800;
		Window::height = 600;

		window = glfwCreateWindow(800, 600, "CR3Converter", nullptr, nullptr);
		if(!window) {
			glfwTerminate();
			return WindowError("Failed to create GLFW window");
		}

		glfwMakeContextCurrent(window);
		glfwSwapInterval(1); // Enable vsync
		if(glewInit() != GLEW_OK) {
			glfwTerminate();
			return WindowError("Failed to initialize GLEW");
		}

		glfwSetFramebufferSizeCallback(window, Window::SetFramebufferSizeCallback);

		return std::nullopt;
	}

	void Window::SetFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);

		Window::width = width;
		Window::height = height;
	}

	void Window::Terminate() {
		glfwTerminate();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
	}

	void Window::Close() {
		s_ShouldClose = true;
	}

	bool Window::ShouldClose() {
		return s_ShouldClose || glfwWindowShouldClose(window);
	}

	void Window::Clear(glm::vec4 color) {
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Window::Update() {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	void Window::RenderImGUI() {
	}

}
