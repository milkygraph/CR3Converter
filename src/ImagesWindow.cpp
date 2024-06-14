#include <Windows/ImagesWindow.hpp>
#include <Core/Errors.hpp>
#include <libraw.h>
#include <future>

namespace CR3Converter {
	void ImagesWindow::Render() {
		ImGui::Begin("Images");

		float available_width = ImGui::GetContentRegionAvail().x; // Available width in the current window
		float current_x = 0.0f; // Track the current x position

		for (auto &image : m_Images) {
			float image_width = 200.0f; // Fixed width for each image
			float image_height = image.height / (image.width / 200.0f); // Maintain aspect ratio

			// Check if there is enough space to fit the image in the current row
			if (current_x + image_width > available_width) {
				// Move to the next line if the image doesn't fit
				ImGui::NewLine();
				current_x = 0.0f; // Reset x position
			} else {
				ImGui::SameLine();
			}

			// Display the image
			ImGui::Image((void *)(intptr_t)image.textureID, ImVec2(image_width, image_height));

			// Advance the current x position
			current_x += image_width + ImGui::GetStyle().ItemSpacing.x * 4.0f;
		}

		ImGui::PopStyleVar();
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

		// Thread pool and mutex for thread safety
		std::vector<std::future<void>> futures;
		std::mutex images_mutex;

		// Function to load an image
		auto load_image = [&](Image& image) {
			LibRaw iProcessor;
			auto path = directory / image.name;
			int result = iProcessor.open_file(path.string().c_str());
			if (result != LIBRAW_SUCCESS) {
				LibRawError("Failed to open file" + path.string());
				return;
			}

			result = iProcessor.unpack();
			if (result != LIBRAW_SUCCESS) {
				LibRawError("Failed to unpack file" + path.string());
				return;
			}

			iProcessor.dcraw_process();
			auto data = iProcessor.dcraw_make_mem_image();
			image.data = data;
			image.width = data->width;
			image.height = data->height;

			// Lock the mutex and push the image to the queue
			{
				std::lock_guard<std::mutex> lock(images_mutex);
				m_GLQueue.push(&image);
			}
		};

		futures.reserve(m_Images.size());
		// Launch a thread for each image
		for (auto &image : m_Images) {
			futures.push_back(std::async(std::launch::async, load_image, std::ref(image)));
		}
	}

	void ImagesWindow::ProcessQueue() {
		while(!m_GLQueue.empty()) {
			auto image = m_GLQueue.front();
			glGenTextures(1, &image->textureID);
			glBindTexture(GL_TEXTURE_2D, image->textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data->data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glBindTexture(GL_TEXTURE_2D, 0);
			m_GLQueue.pop();
			break;
		}
	}
}