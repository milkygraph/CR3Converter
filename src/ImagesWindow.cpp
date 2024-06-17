#include <Windows/ImagesWindow.hpp>
#include <Core/Errors.hpp>
#include <libraw.h>
#include <future>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace CR3Converter {
	void LoadImage(std::queue<Image*>& gl_queue,
				   const std::filesystem::path& directory, Image& image) {
		LibRaw iProcessor;
		auto path = directory / image.name;
		int result = iProcessor.open_file(path.string().c_str());
		if (result != LIBRAW_SUCCESS) {
			LibRawError("Failed to open file" + path.string());
			return;
		}

		result = iProcessor.unpack_thumb();
		if (result != LIBRAW_SUCCESS) {
			LibRawError("Failed to unpack file" + path.string());
			return;
		}

		auto data = iProcessor.dcraw_make_mem_thumb();
		if (data == nullptr) {
			LibRawError("Failed to make thumbnail for file" + path.string());
			return;
		}

		if(iProcessor.imgdata.thumbnail.tformat == LIBRAW_THUMBNAIL_JPEG) {
			// Load the JPEG image
			int width, height, channels;
			unsigned char* jpeg_data = stbi_load_from_memory(data->data, iProcessor.imgdata.thumbnail.tlength, &width, &height, &channels, 0);
			if(jpeg_data == nullptr) {
				LibRawError("Failed to load JPEG thumbnail for file" + path.string());
				return;
			}
			image.data = (char*)jpeg_data;
		} else {
			LibRawError("Unsupported thumbnail format for file" + path.string());
			return;
		}

		image.width = iProcessor.imgdata.thumbnail.twidth;
		image.height = iProcessor.imgdata.thumbnail.theight;

		std::mutex images_mutex;
		{
			std::lock_guard<std::mutex> lock(images_mutex);
			gl_queue.push(&image);
		}

		iProcessor.recycle();
	}

	void ImagesWindow::Render() {
		ImGui::Begin("Images");

		float available_width = ImGui::GetContentRegionAvail().x; // Available width in the current window
		float image_width = 200.0f; // Fixed width for each image
		ImGui::Columns(available_width / image_width, nullptr, false);

		for (auto &image : m_Images) {
			// If it is a new row, put a padding
			if (ImGui::GetColumnIndex() == 0)
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetTextLineHeight());

			float image_height = image.height / (image.width / 200.0f); // Maintain aspect ratio
			if(ImGui::InvisibleButton(image.name.c_str(), ImVec2(image_width, image_height)) && image.textureID != 0) {
				std::cout << "Image clicked: " << image.name << std::endl;
			}
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - (image_height + ImGui::GetTextLineHeight()));
			ImGui::Image((void*)(intptr_t)image.textureID, ImVec2(image_width, image_height));

			// Center the text
			float text_width = ImGui::CalcTextSize(image.name.c_str()).x;
			float cursor_x = ImGui::GetCursorPosX();
			ImGui::SetCursorPosX(cursor_x + (image_width - text_width) / 2);
			ImGui::Text("%s", image.name.c_str());

			ImGui::NextColumn();
		}
		ImGui::Columns(1);
		ImGui::End();
	}

	void ImagesWindow::LoadImages(const std::filesystem::path &directory) {
		// Delete existing textures
		for (auto &image: m_Images)
			glDeleteTextures(1, &image.textureID);

		m_Images.clear();

		// Count the number of valid image files
		for (const auto &entry : std::filesystem::directory_iterator(directory)) {
			if (entry.is_regular_file() && (entry.path().extension() == ".CR3" || entry.path().extension() == ".cr3")) {
				m_Images.push_back({nullptr, 0, entry.path().filename(), 0, 0});
			}
		}

		// Launch a thread for each image
		for (auto &image : m_Images) {
			m_Processor.Enqueue(LoadImage, std::ref(m_GLQueue), directory, std::ref(image));
		}
	}

	void ImagesWindow::ProcessQueue() {
		while(!m_GLQueue.empty()) {
			auto image = m_GLQueue.front();
			glGenTextures(1, &image->textureID);
			glBindTexture(GL_TEXTURE_2D, image->textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glBindTexture(GL_TEXTURE_2D, 0);
			free(image->data);
			m_GLQueue.pop();
			break;
		}
	}
}