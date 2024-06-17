#pragma once
#include <Components/Image.hpp>
#include <future>
#include <queue>
#include <libraw.h>

namespace CR3Converter {
	class ImageProcessor {
		public:
			ImageProcessor();
			~ImageProcessor();

			template<typename F, typename... Args>
			void Enqueue(F&& function, Args&&... args);
		private:
			std::vector<std::thread> m_ProcessingThreads;
			std::queue<std::function<void()>> m_ProcessingQueue;
			std::mutex m_QueueMutex;
			std::condition_variable m_QueueCondition;
			bool m_Stop;
	};

	template<typename F, typename... Args>
	void ImageProcessor::Enqueue(F &&function, Args &&...args) {
		{
			std::lock_guard<std::mutex> lock(m_QueueMutex);
			m_ProcessingQueue.emplace([function, args...]() {
				function(args...);
			});
		}
		m_QueueCondition.notify_one();
	}
}

