#include "thread_pool.h"

namespace wx2
{

	ThreadPool::ThreadPool(const std::size_t threadCount) noexcept
		: status_(Status::Standby)
		, threadCount_(threadCount)
		, taskQueue_(false)
	{
		std::unique_lock<std::mutex> lock(mutex_);

		status_ = Status::Working;

		threads_.clear();
		threads_.reserve(threadCount_);
		for (std::size_t i = 0; i < threadCount_; ++i)
			threads_.emplace_back([this] { Join(); });
	}

	ThreadPool::~ThreadPool() noexcept
	{
		{
			std::unique_lock<std::mutex> lock(mutex_);
			status_ = Status::Joining;
		}
		condition_.notify_all();

		for (auto& thread : threads_)
			thread.join();

		threads_.clear();
	}

	void ThreadPool::Join() noexcept
	{
		while (true)
		{
			auto work = Dequeue();

			if (work.has_value())
			{
				work.value()();
			}
			else
				return;
		}
	}

	std::optional<std::function<void()>> ThreadPool::Dequeue()
	{
		std::unique_lock<std::mutex> lock(mutex_);
		while (taskQueue_.empty())
		{
			if (status_ == Status::Joining)
			{
				return std::nullopt;
			}
			condition_.wait(lock);
		}

		std::function<void()> proc = std::move(taskQueue_.front());
		taskQueue_.pop_front();

		return proc;
	}

}