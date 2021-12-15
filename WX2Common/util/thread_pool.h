/*********************************************************************
 * @file	thread_pool.h
 * @author	Tomomi Murakami
 * @date	2021/06/27 17:48
 * @brief   �X���b�h�v�[��
*********************************************************************/
#pragma once
#include <deque>
#include <future>
#include <thread>
#include <optional>
#include "macro.h"

namespace wx2
{
	/**
	 * @brief  �X���b�h�v�[���N���X
	 */
	class ThreadPool final
	{
	private:
		// �ғ����
		enum class Status
		{
			Standby, Working, Joining,
		};

	public:
		/**
		 * @brief  ���蓖�Ă�X���b�h�����w�肵�ăX���b�h������������
		 * @param  threadCount ���蓖�Ă�X���b�h��
		 */
		explicit ThreadPool(std::size_t threadCount = std::thread::hardware_concurrency()) noexcept;
		//! @brief  �X���b�h��Join���� 
		~ThreadPool() noexcept;

		// �R�s�[�ƃ��[�u�֎~
		WX2_DISALLOW_COPY_AND_MOVE(ThreadPool);

		template<typename F, typename... Args>
		auto Enqueue(F&& function, Args&&... args) noexcept
			-> std::future<std::invoke_result_t<F, Args...>>
		{
			using TaskReturnType = std::invoke_result_t<F, Args...>;
			using TaskType = std::packaged_task<TaskReturnType()>;

			auto task = std::make_shared<TaskType>(std::bind(std::forward<F>(function), std::forward<Args>(args)...));
			auto result = task->get_future();

			{
				std::lock_guard<std::mutex> lock(mutex_);
				taskQueue_.emplace_back([task]() { (*task)(); });
			}
			condition_.notify_all();
			return result;
		}

		[[nodiscard]] std::size_t NumThread() const { return threadCount_; }

	private:
		void Join() noexcept;

		[[nodiscard]] std::optional<std::function<void()>> Dequeue();

		Status status_;
		std::size_t threadCount_;
		std::vector<std::thread> threads_;
		std::mutex mutex_;
		std::condition_variable condition_;
		std::deque<std::function<void()>> taskQueue_;
	};

}
