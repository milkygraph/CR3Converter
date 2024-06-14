#pragma once
#include <GL/glew.h>
#include <vector>
#include <queue>
#include <imgui.h>
#include <filesystem>
#include <libraw.h>
#include <functional>

struct Image {
	libraw_processed_image_t* data;
	unsigned int textureID;
	std::string name;
	int width;
	int height;
};

namespace CR3Converter {
	class ImagesWindow {
		public:
			void LoadImages(const std::filesystem::path& directory);
			void Render();
			void ProcessQueue();
	private:
		std::vector<Image> m_Images; // Vector of images
		std::queue<Image*> m_GLQueue; // Queue of OpenGL commands
	};
}