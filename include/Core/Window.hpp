#pragma once
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <optional>

#include "Core/Errors.hpp"

namespace CR3Converter {
    class Window {
        public:
            static std::optional<WindowError> Init();
            static void Terminate();
	        static bool ShouldClose();
            static void Update();
			static void RenderImGUI();
            static void Clear(glm::vec4 color = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
			static GLFWwindow* GetWindow() { return window; }
        private:
			static std::optional<WindowError> InitGLFW();
	        static std::optional<WindowError> InitImGUI();
            static void SetFramebufferSizeCallback(GLFWwindow* window, int width, int height);
            static GLFWwindow* window;
            static int width, height;
    };
}
