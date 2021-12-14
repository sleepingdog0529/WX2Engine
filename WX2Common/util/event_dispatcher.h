/*********************************************************************
 * @file   event_dispatcher.h
 * @author Tomomi Murakami
 * @date   2021/12/12 15:10
 * @brief  イベント発行機
 ********************************************************************/
#pragma once
#include <functional>
#include <unordered_map>
#include <unordered_set>

namespace wx2
{
	template <typename Key, typename... Args>
	class EventDispatcher;

	template <typename Key, typename... Args>
	class Event final
	{
		using EventDispatcherType = EventDispatcher<Key, Args...>;
		using CallbackFunc = std::function<void(Args...)>;

		friend class EventDispatcherType;

	public:
		// コンストラクタ＆デストラクタ
		Event() = default;
		Event(std::initializer_list<CallbackFunc> callbacks)
		{
			callbacks_.insert(callbacks.begin(), callbacks.end());
		}
		~Event() = default;

		// ムーブ＆コピー
		Event(const Event& other) = default;
		Event& operator=(const Event& other) = default;
		Event(Event&& other) = default;
		Event& operator=(Event&& other) = default;

		void AddCallback(const Key& key, CallbackFunc function)
		{
			callbacks_.emplace(key, std::move(function));
		}

		void Clear()
		{
			for (auto& subject : eventDispatchers_)
			{
				if (subject)
				{
					subject->RemoveCallback(*this);
				}
			}
		}

	private:
		void OnDispatch(const Key& key, Args&&... args)
		{
			auto [itr, end] = callbacks_.equal_range(key);
			for (;itr != end; ++itr)
			{
				itr->second(std::forward<Args>(args)...);
			}
		}

		std::unordered_multimap<Key, CallbackFunc> callbacks_{};
		std::unordered_set<EventDispatcherType*> eventDispatchers_{};
	};

	template <typename Key, typename... Args>
	class EventDispatcher final
	{
		using EventType = Event<Key, Args...>;

	public:
		EventDispatcher() = default;
		~EventDispatcher() = default;

		// ムーブ＆コピー
		EventDispatcher(const EventDispatcher& other) = default;
		EventDispatcher& operator=(const EventDispatcher& other) = default;
		EventDispatcher(EventDispatcher&& other) = default;
		EventDispatcher& operator=(EventDispatcher&& other) = default;

		void Dispatch(const Key& key, Args&&... args)
		{
			for (auto& observer : events_)
			{
				observer->OnDispatch(key, std::forward<Args>(args)...);
			}
		}

		void AddCallback(const EventType& event)
		{
			event.eventDispatchers_.insert(this);
			events_.emplace_back(&event);
		}

		void RemoveCallback(const EventType& event)
		{
			std::erase_if(
				events_, 
				[&event](const EventType* cb) { return cb == &event; });
			event.eventDispatchers_.erase(this);
		}

	private:
		std::vector<EventType*> events_{};
	};
}
