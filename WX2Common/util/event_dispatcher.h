/*********************************************************************
 * @file   event_dispatcher.h
 * @author Tomomi Murakami
 * @date   2021/12/12 15:10
 * @brief  �C�x���g���s�@�ƃC�x���g���X�i�[
 ********************************************************************/
#pragma once
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <unordered_set>

namespace wx2
{
	// �O���錾
	template <typename EventType, typename... Args>
	class EventDispatcher;

	/**
	 * @brief  �C�x���g���X�i�[
	 * @tparam EventType �C�x���g�̎�ނ𔻕ʂ��邽�߂̃L�[�̌^
	 * @tparam Args �R�[���o�b�N�ɓn���������̌^
	 */
	template <typename EventType, typename... Args>
	class EventListener final
	{
		//! �Ή������C�x���g���s�N���X�̌^
		using EventDispatcherType = EventDispatcher<EventType, Args...>;
		//! �R�[���o�b�N�֐��^
		using CallbackFunc = std::function<void(Args...)>;

		//! �C�x���g���s�N���X�̃A�N�Z�X������
		friend class EventDispatcherType;

	public:
		EventListener() = default;
		/**
		 * @brief �R�[���o�b�N�֐����Z�b�g����
		 * @param callback �R�[���o�b�N�֐�
		 */
		EventListener(CallbackFunc callback)
			: callback_(std::move(callback)) {}
		~EventListener() = default;

		// ���[�u�ƃR�s�[������
		EventListener(const EventListener& other) = default;
		EventListener& operator=(const EventListener& other) = default;
		EventListener(EventListener&& other) = default;
		EventListener& operator=(EventListener&& other) = default;

		/**
		 * @brief �R�[���o�b�N�֐����Z�b�g����
		 * @param callback �R�[���o�b�N�֐�
		 */
		void SetCallback(const CallbackFunc& callback) noexcept
		{
			callback_ = callback;
		}

		/**
		 * @brief �R�[���o�b�N�̓o�^����������
		 */
		void Clear() noexcept
		{
			// ���g���o�^����Ă���S�ẴC�x���g���s�N���X���玩�g��o�^��������
			for (auto& dispatcher : eventDispatchers_)
			{
				if (dispatcher)
				{
					dispatcher->RemoveEventListener(*this);
				}
			}
		}

		/**
		 * @brief  �R�[���o�b�N�֐����Ăяo���\�����ׂ�
		 * @return �Ăяo���\��
		 */
		[[nodiscard]] bool HasCallback() const noexcept { return !!callback_; }

	private:
		/**
		 * @brief  �R�[���o�b�N���Ăяo��
		 * @param  args �R�[���o�b�N�̈���
		 */
		void OnDispatch(const Args&... args)
		{
			callback_(args...);
		}

		//! �R�[���o�b�N�֐�
		CallbackFunc callback_{};
		//! ���g���o�^����Ă���S�ẴC�x���g���s�N���X
		std::unordered_set<EventDispatcherType*> eventDispatchers_{};
	};

	/**
	 * @brief  �C�x���g���s�@
	 * @tparam EventType �C�x���g�̎�ނ𔻒肷�邽�߂̃L�[�̌^
	 * @tparam Args �R�[���o�b�N�ɓn���������̌^
	 */
	template <typename EventType, typename... Args>
	class EventDispatcher
	{
		//! �C�x���g���X�i�[�̌^
		using EventListenerType = EventListener<EventType, Args...>;

	public:
		EventDispatcher() = default;
		virtual ~EventDispatcher() = default;

		// ���[�u�ƃR�s�[������
		EventDispatcher(const EventDispatcher& other) = default;
		EventDispatcher& operator=(const EventDispatcher& other) = default;
		EventDispatcher(EventDispatcher&& other) = default;
		EventDispatcher& operator=(EventDispatcher&& other) = default;

		/**
		 * @brief �C�x���g�𔭍s����
		 * @param event �C�x���g�̎��
		 * @param args �R�[���o�b�N�ɓn������
		 */
		void Dispatch(const EventType& event, const Args&... args)
		{
			auto [itr, end] = listeners_.equal_range(event);
			for (; itr != end; ++itr)
			{
				itr->second->OnDispatch(args...);
			}
		}

		/**
		 * @brief �C�x���g���X�i�[��o�^����
		 * @param event �C�x���g�̎��
		 * @param eventListener �o�^����C�x���g���X�i�[
		 */
		void AddEventListener(EventType&& event, EventListenerType& eventListener)
		{
			eventListener.eventDispatchers_.insert(this);
			listeners_.emplace(std::forward<EventType>(event), &eventListener);
		}

		/**
		 * @brief �C�x���g���X�i�[�����O����
		 * @param event ���O����C�x���g�̎��
		 * @param eventListener ���O����C�x���g���X�i�[
		 */
		void RemoveEventListener(const EventType& event, EventListenerType& eventListener)
		{
			std::erase_if(
				listeners_,
				[&](const auto& l) { return l.first == event && l.second == &eventListener; });
			eventListener.eventDispatchers_.erase(this);
		}

	private:
		//! �C�x���g���X�i�[�̘A�z�z��
		std::unordered_multimap<EventType, EventListenerType*> listeners_{};
	};
}