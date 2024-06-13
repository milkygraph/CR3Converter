#include <Core/Window.hpp>

int main() {
    // Initialize GLFW
    auto result = CR3Converter::Window::Init();
	if(result.has_value()) {
		return 1;
	}

	auto window = CR3Converter::Window::GetWindow();
	auto io = ImGui::GetIO(); (void)io;

    // Main loop
    while (!CR3Converter::Window::ShouldClose()) {
	    // Start the ImGui frame
	    ImGui_ImplOpenGL3_NewFrame();
	    ImGui_ImplGlfw_NewFrame();
	    ImGui::NewFrame();

	    // ImGui content
	    ImGui::Begin("Debug Window");
	    ImGui::Text("Hello, world!");
	    ImGui::End();

		CR3Converter::Window::Clear();
		CR3Converter::Window::RenderImGUI();
		CR3Converter::Window::Update();
    }

    // Terminate Window
    CR3Converter::Window::Terminate();
    return 0;
}
