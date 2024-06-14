#include <imgui.h>
#include <Core/Window.hpp>
#include <Windows/ImagesWindow.hpp>
#include <Core/Errors.hpp>
#include <filesystem>

namespace CR3Converter {
	class GUI {
		public:
			static std::optional<GUIError> Init(GLFWwindow* window);
			static void BeginFrame();
			static void Render();
			static void Terminate();
		private:
			static ImagesWindow m_ImagesWindow;
			static std::filesystem::path m_CurrentDirectory;
	};
}