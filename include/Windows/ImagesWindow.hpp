#pragma once
#include <GL/glew.h>
#include <vector>
#include <queue>
#include <imgui.h>
#include <filesystem>
#include <libraw.h>
#include <functional>
#include <Components/Image.hpp>
#include <Core/ImageProcessor.hpp>

namespace CR3Converter {
	class ImagesWindow {
		public:
			void LoadImages(const std::filesystem::path& directory);
			void Render();
			void ProcessQueue();
	private:
		ImageProcessor m_Processor;
		std::vector<Image> m_Images; // Vector of images
		std::queue<Image*> m_GLQueue; // Queue of OpenGL commands
	};
}