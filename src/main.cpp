#include <Core/Window.hpp>
#include <libraw.h>

int main() {
    // Initialize GLFW
    auto result = CR3Converter::Window::Init();
	if(result.has_value()) {
		return 1;
	}

	LibRaw iProcessor;
	int r = iProcessor.open_file("example.CR3");
	if(r != LIBRAW_SUCCESS) {
		LibRawError("Failed to open file");
		return 1;
	}

	r = iProcessor.unpack();
	if(r != LIBRAW_SUCCESS) {
		LibRawError("Failed to unpack file");
		return 1;
	}

	// Dump the raw data into a vector
	iProcessor.dcraw_process();
	auto data = iProcessor.dcraw_make_mem_image();
	auto width = data->width;
	auto height = data->height;

	// Create a texture from the raw data
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data->data);
	glGenerateMipmap(GL_TEXTURE_2D);

// Set the texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);

    // Main loop
    while (!CR3Converter::Window::ShouldClose()) {
	    // Start the ImGui frame
	    ImGui_ImplOpenGL3_NewFrame();
	    ImGui_ImplGlfw_NewFrame();
	    ImGui::NewFrame();

	    // ImGui content
	    ImGui::Begin("Raw Image Viewer");

	    ImVec2 imageSize(width / 10, height / 10);
	    ImGui::Image((void*)(intptr_t)textureID, imageSize);

	    ImGui::End();

	    CR3Converter::Window::Clear();
	    CR3Converter::Window::RenderImGUI();
	    CR3Converter::Window::Update();
    }

    // Terminate Window
    CR3Converter::Window::Terminate();
    return 0;
}