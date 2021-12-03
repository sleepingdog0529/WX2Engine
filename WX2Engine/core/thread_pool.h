/*********************************************************************
 * @file	thread_pool.h
 * @author	Tomomi Murakami
 * @date	2021/06/27 17:48
*********************************************************************/
#pragma once
#include <future>

namespace WX2
{

	class ThreadPool final
	{
	public:
		explicit ThreadPool(int thread_count = std::thread::hardware_concurrency());
		~ThreadPool();

		template<typename F, typename... Args>
		auto Enqueue(F&& function, Args&&... args)
			-> std::future<std::invoke_result_t<F, Args...>>
		{
			using TaskReturnType = std::invoke_result_t<F, Args...>;
			using TaskType = std::packaged_task<TaskReturnType()>;

			auto task = std::make_shared<TaskType>(std::bind(std::forward<F>(function), std::forward<Args>(args)...));
			auto result = task->get_future();

			{
				std::lock_guard<std::mutex> lock(mutex_);
				task_queue_.emplace_back([task]() { (*task)(); });
			}
			condition_.notify_all();
			return result;
		}

		UINT NumThread() const { return thread_count_; }

	private:
		void Join();

		std::function<void()> Dequeue();

		struct ThreadTerminator {};

		enum Status
		{
			STANDBY, WORKING, JOINING,
		};

		Status status_;
		UINT thread_count_;
		std::vector<std::thread> threads_;
		std::mutex mutex_;
		std::condition_variable condition_;
		std::deque<std::function<void()>> task_queue_;
	};

}
