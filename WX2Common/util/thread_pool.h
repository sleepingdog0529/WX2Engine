/*********************************************************************
 * @file	thread_pool.h
 * @author	Tomomi Murakami
 * @date	2021/06/27 17:48
 * @brief   スレッドプール
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
	 * @brief  スレッドプールクラス
	 */
	class ThreadPool final
	{
	private:
		// 稼働状態
		enum class Status
		{
			Standby, Working, Joining,
		};

	public:
		/**
		 * @brief  割り当てるスレッド数を指定してスレッドを初期化する
		 * @param  threadCount 割り当てるスレッド数
		 */
		explicit ThreadPool(std::size_t threadCount = std::thread::hardware_concurrency()) noexcept;
		//! @brief  スレッドをJoinする 
		~ThreadPool() noexcept;

		// コピーとムーブ禁止
		WX2_DISALLOW_COPY_AND_MOVE(ThreadPool);

		/**
		 * @brief  タスクキューに関数をプッシュする
		 * @tparam F タスク関数型
		 * @tparam Args 引数型
		 * @param  function タスク関数
		 * @param  args 引数
		 * @return 共有状態を持つ処理結果
		 */
		template<typename F, typename... Args>
		auto Enqueue(F&& function, Args&&... args) noexcept
			-> std::future<std::invoke_result_t<F, Args...>>
		{
			// 関数の戻り値型
			using TaskReturnType = std::invoke_result_t<F, Args...>;
			// 非同期実行する関数を登録した型
			using TaskType = std::packaged_task<TaskReturnType()>;

			// 共有状態を初期化しタスクを格納する
			auto task = std::make_shared<TaskType>(std::bind(std::forward<F>(function), std::forward<Args>(args)...));
			// 共有状態を持つ処理結果を取得する
			auto result = task->get_future();

			// タスクキューにタスクをプッシュ
			{
				std::lock_guard<std::mutex> lock(mutex_);
				taskQueue_.emplace_back([task]() { (*task)(); });
			}

			// スレッドの稼働再開
			condition_.notify_all();

			return result;
		}

		/**
		 * @brief  使用しているスレッド数を返す
		 * @return スレッド数
		 */
		[[nodiscard]] std::size_t NumThread() const { return threadCount_; }

	private:
		/**
		 * @brief スレッドにタスクを与える
		 */
		void Join() noexcept;


		/**
		 * @brief  タスクをタスクキューからポップする
		 * @return タスク
		 */
		[[nodiscard]] std::optional<std::function<void()>> Dequeue();

		Status status_;						//! 稼働状態
		std::size_t threadCount_;			//! 使用スレッド数
		std::vector<std::thread> threads_;	//! スレッドリスト
		std::mutex mutex_;					//! 排他制御をロックする
		std::condition_variable condition_;				//! タスクキューの状態
		std::deque<std::function<void()>> taskQueue_;	//! タスクキュー
	};

}
