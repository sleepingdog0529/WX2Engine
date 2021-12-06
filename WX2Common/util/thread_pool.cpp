#include "thread_pool.h"
#include "thread_pool.h"

namespace wx2
{

	ThreadPool::ThreadPool(const int threadCount) noexcept
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
		try
		{
			while (true)
			{
				if (std::function<void()> work = Dequeue())
				{
					work();
				}
			}
		}
		catch (const ThreadTerminator&)
		{
			return;
		}
	}

	std::function<void()> ThreadPool::Dequeue()
	{
		std::unique_lock<std::mutex> lock(mutex_);
		while (taskQueue_.empty())
		{
			if (status_ == Status::Joining)
			{
				throw ThreadTerminator();
			}
			condition_.wait(lock);
		}

		std::function<void()> proc = std::move(taskQueue_.front());
		taskQueue_.pop_front();

		return proc;
	}

}