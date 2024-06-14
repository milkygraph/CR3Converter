#include <Core/Window.hpp>
#include <Core/GUI.hpp>
#include <libraw.h>

using namespace CR3Converter;

int main() {
    // Initialize GLFW
	if(Window::Init().has_value()) {
		return 1;
	}

	// Initialize GUI
	if(GUI::Init(Window::GetGLFWWindow()).has_value()) {
		return 1;
	}

    // Main loop
    while (!Window::ShouldClose()) {
		GUI::BeginFrame();
	    Window::Clear();
	    GUI::Render();
	    Window::Update();
    }

	// Terminate GUI
	GUI::Terminate();

    // Terminate Window
    Window::Terminate();

    return 0;
}