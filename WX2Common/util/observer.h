/*********************************************************************
 * @file   observer.h
 * @author Tomomi Murakami
 * @date   2021/12/12 15:10
 * @brief  オブザーバーとサブジェクトクラス
 ********************************************************************/
#pragma once
#include <functional>
#include <unordered_map>
#include <unordered_set>

namespace wx2
{
	template <typename Key, typename... Args>
	class Subject;

	template <typename Key, typename... Args>
	class Observer final
	{
		using SubjectType = Subject<Key, Args...>;
		using SubscriberFunc = std::function<void(Args...)>;

		friend class SubjectType;

	public:
		// コンストラクタ＆デストラクタ
		Observer() = default;
		~Observer() = default;

		// ムーブ＆コピー
		Observer(const Observer& other) = default;
		Observer& operator=(const Observer& other) = default;
		Observer(Observer&& other) = default;
		Observer& operator=(Observer&& other) = default;

		void SetFunction(const Key& key, SubscriberFunc function)
		{
			subscribers_.emplace(key, std::move(function));
		}

		void Clear()
		{
			for (auto& subject : subjects_)
			{
				if (subject)
				{
					subject->RemoveObserver(*this);
				}
			}
			subjects_.clear();
		}

	private:
		void OnNotify(const Key& key, Args&&... args)
		{
			auto [itr, end] = subscribers_.equal_range(key);
			for (;itr != end; ++itr)
			{
				itr->second(std::forward<Args>(args)...);
			}
		}

		std::unordered_multimap<Key, SubscriberFunc> subscribers_{};
		std::unordered_set<SubjectType*> subjects_{};
	};

	template <typename Key, typename... Args>
	class Subject final
	{
		using ObserverType = Observer<Key, Args...>;

	public:
		Subject() = default;
		~Subject() = default;

		// ムーブ＆コピー
		Subject(const Subject& other) = default;
		Subject& operator=(const Subject& other) = default;
		Subject(Subject&& other) = default;
		Subject& operator=(Subject&& other) = default;

		void Notify(const Key& key, Args&&... args)
		{
			for (auto& observer : observers_)
			{
				observer->OnNotify(key, std::forward<Args>(args)...);
			}
		}

		void AddObserver(ObserverType& observer)
		{
			observer.subjects_.insert(this);
			observers_.emplace_back(&observer);
		}

		void RemoveObserver(const ObserverType& observer)
		{
			std::erase_if(
				observers_, 
				[&observer](const ObserverType* obs) { return obs == &observer; });
		}

	private:
		std::vector<ObserverType*> observers_{};
	};
}
