/*********************************************************************
 * @file   transform.h
 * @author Tomomi Murakami
 * @date   2021/12/19 20:55
 * @brief  épê®êßå‰
 ********************************************************************/
#pragma once
#include "core.h"

namespace wx2
{
	class Transform
	{
	public:
		Transform() = default;
		virtual ~Transform() = default;

		WX2_COPYABLE(Transform);
		WX2_MOVEABLE(Transform);

		void SetPositon(const Vector3& position) noexcept { position_ = position; }
		void SetPositon(const float x, const float y, const float z) noexcept { position_ = { x, y, z }; }
		void AddPositon(const Vector3& position) noexcept { position_ += position; }
		void AddPositon(const float x, const float y, const float z) noexcept { position_ += { x, y, z }; }

		void SetRotation(const Quaternion& rotation) noexcept { rotation_ = rotation; }
		void SetLookAt(const Vector3& target, const Vector3& up = Vector3::Up()) noexcept;
		void SetLookTo(const Vector3& forward, const Vector3& up = Vector3::Up()) noexcept;
		void AddRotation(const Quaternion& rotation) noexcept { rotation_ *= rotation; }

		void SetScale(const Vector3& scale) noexcept { scale_ = scale; }
		void SetScale(const float x, const float y, const float z) noexcept { scale_ = { x, y, z }; }
		void AddScale(const Vector3& scale) noexcept { scale_ += scale; }
		void AddScale(const float x, const float y, const float z) noexcept { scale_ += { x, y, z }; }

		[[nodiscard]] const Vector3& GetPositon() const noexcept { return position_; }
		[[nodiscard]] const Quaternion& GetRotation() const noexcept { return rotation_; }
		[[nodiscard]] const Vector3& GetScale() const noexcept { return scale_; }

		[[nodiscard]] const Vector3& Right()    const noexcept { return rotation_.Right(); }
		[[nodiscard]] const Vector3& Left()     const noexcept { return rotation_.Left(); }
		[[nodiscard]] const Vector3& Up()       const noexcept { return rotation_.Up(); }
		[[nodiscard]] const Vector3& Down()     const noexcept { return rotation_.Down(); }
		[[nodiscard]] const Vector3& Forward()  const noexcept { return rotation_.Forward(); }
		[[nodiscard]] const Vector3& Backward() const noexcept { return rotation_.Backward(); }

		[[nodiscard]] Matrix GetWorldMatrix() const noexcept;
		[[nodiscard]] Matrix GetViewMatrix() const noexcept;

	private:
		Vector3 position_{};
		Quaternion rotation_{};
		Vector3 scale_{};
	};
}
