#include <Core/GUI.hpp>
#include <Core/Window.hpp>

namespace CR3Converter {
	ImagesWindow GUI::m_ImagesWindow;
	std::filesystem::path GUI::m_CurrentDirectory = std::filesystem::current_path();

	std::optional<GUIError> GUI::Init(GLFWwindow* window) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui::StyleColorsDark();

		if(!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
			GUIError("Failed to initialize ImGUI for OpenGL");
		}

		if(!ImGui_ImplOpenGL3_Init("#version 330")) {
			GUIError("Failed to initialize OpenGL3 for ImGUI");
		}

		m_ImagesWindow.LoadImages(m_CurrentDirectory);

		return std::nullopt;
	}

	void GUI::BeginFrame() {
		m_ImagesWindow.ProcessQueue();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport();
	}

	void GUI::Render() {
		// Menu bar
		if(ImGui::BeginMainMenuBar()) {
			if(ImGui::BeginMenu("File")) {
				if(ImGui::MenuItem("Open Folder")) {
					// TODO: Open folder dialog
					m_CurrentDirectory = "/mnt/Data/Photos/100CANON/";
					m_ImagesWindow.LoadImages(m_CurrentDirectory);
				}
				if(ImGui::MenuItem("Exit")) {
					Window::Close();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// Windows
		m_ImagesWindow.Render();


		// Render ImGUI
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void GUI::Terminate() {
	}
}