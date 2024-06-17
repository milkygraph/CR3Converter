#include <Core/ImageProcessor.hpp>
#include <functional>
#include <iostream>

namespace CR3Converter {
	ImageProcessor::ImageProcessor() : m_Stop(false) {
		// Initialize worker threads
		m_ProcessingThreads.reserve(std::thread::hardware_concurrency());
		for (size_t i = 0; i < m_ProcessingThreads.capacity(); i++) {
			m_ProcessingThreads.emplace_back([this]() {
				while (!m_Stop) {
					std::function<void()> task;
					{
						std::unique_lock<std::mutex> lock(m_QueueMutex);
						m_QueueCondition.wait(lock, [this]() { return m_Stop || !m_ProcessingQueue.empty(); });
						if (m_Stop) {
							return;
						}
						task = std::move(m_ProcessingQueue.front());
						m_ProcessingQueue.pop();
					}
					task();
				}
			});
		}
	}

	ImageProcessor::~ImageProcessor() {
		// Stop worker threads
		{
			std::lock_guard<std::mutex> lock(m_QueueMutex);
			m_Stop = true;
		}
		m_QueueCondition.notify_all();
		for (auto &thread : m_ProcessingThreads) {
			thread.join();
		}
	}
}