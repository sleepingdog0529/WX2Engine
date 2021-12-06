#include "vector3.h"
#pragma once

namespace wx2
{
	inline bool Vector3::operator==(const Vector3& rhs) const noexcept
	{
		return DirectX::XMVector3Equal(vector_, rhs.vector_);
	}

	inline bool Vector3::operator!=(const Vector3& rhs) const noexcept
	{
		return DirectX::XMVector3NotEqual(vector_, rhs.vector_);
	}

	inline Vector3 Vector3::operator-() const noexcept
	{
		Vector3 v;
		v.vector_ = DirectX::XMVectorNegate(vector_);
		return v;
	}

	inline Vector3 Vector3::operator+(const Vector3& rhs) const noexcept
	{
		return Vector3();
	}

	inline Vector3 Vector3::operator-(const Vector3& rhs) const noexcept
	{
		return Vector3();
	}

	inline Vector3 Vector3::operator*(const float rhs) const noexcept
	{
		return Vector3();
	}

	inline Vector3 Vector3::operator/(const float rhs) const noexcept
	{
		return Vector3();
	}

	inline Vector3& Vector3::operator+=(const Vector3& rhs) noexcept
	{
		// TODO: return ステートメントをここに挿入します
	}

	inline Vector3& Vector3::operator-=(const Vector3& rhs) noexcept
	{
		// TODO: return ステートメントをここに挿入します
	}

	inline Vector3& Vector3::operator*=(const float rhs) noexcept
	{
		// TODO: return ステートメントをここに挿入します
	}

	inline Vector3& Vector3::operator/=(const float rhs) noexcept
	{
		// TODO: return ステートメントをここに挿入します
	}

	inline float Vector3::Length() const noexcept
	{
		return DirectX::XMVectorGetX(DirectX::XMVector3Length(vector_));
	}

	inline float Vector3::LengthSquared() const noexcept
	{
		return DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(vector_));
	}
}