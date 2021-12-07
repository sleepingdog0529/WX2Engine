#pragma once
#include "vector3.h"
#include <WX2Common.h>

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
		return DirectX::XMVectorNegate(vector_);
	}

	inline Vector3 Vector3::operator+(const Vector3& rhs) const noexcept
	{
		return DirectX::XMVectorAdd(vector_, rhs.vector_);
	}

	inline Vector3 Vector3::operator-(const Vector3& rhs) const noexcept
	{
		return DirectX::XMVectorSubtract(vector_, rhs.vector_);
	}

	inline Vector3 Vector3::operator*(const float rhs) const noexcept
	{
		return DirectX::XMVectorScale(vector_, rhs);
	}

	inline Vector3 Vector3::operator/(const float rhs) const noexcept
	{
		WX2_ASSERT_MSG(rhs != 0.0f, "0èúéZÇ™î≠ê∂ÇµÇ‹ÇµÇΩÅB");
		return DirectX::XMVectorScale(vector_, 1.0f / rhs);
	}

	inline Vector3& Vector3::operator+=(const Vector3& rhs) noexcept
	{
		vector_ = DirectX::XMVectorAdd(vector_, rhs.vector_);
		return *this;
	}

	inline Vector3& Vector3::operator-=(const Vector3& rhs) noexcept
	{
		vector_ = DirectX::XMVectorSubtract(vector_, rhs.vector_);
		return *this;
	}

	inline Vector3& Vector3::operator*=(const float rhs) noexcept
	{
		vector_ = DirectX::XMVectorScale(vector_, rhs);
		return *this;
	}

	inline Vector3& Vector3::operator/=(const float rhs) noexcept
	{
		WX2_ASSERT_MSG(rhs != 0.0f, "0èúéZÇ™î≠ê∂ÇµÇ‹ÇµÇΩÅB");
		vector_ = DirectX::XMVectorScale(vector_, 1.0f / rhs);
		return *this;
	}

	inline float Vector3::operator[](const std::size_t index) const noexcept
	{
		WX2_ASSERT_MSG(index < 3, "ìYÇ¶éöÇÃílÇ™îÕàÕäOÇ≈Ç∑ÅB");
		return vector_.m128_f32[index];
	}

	inline void Vector3::Normalized() noexcept
	{
		vector_ = DirectX::XMVector3Normalize(vector_);
	}

	inline float Vector3::Length() const noexcept
	{
		return DirectX::XMVectorGetX(DirectX::XMVector3Length(vector_));
	}

	inline float Vector3::LengthSquared() const noexcept
	{
		return DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(vector_));
	}

	inline Vector3 Vector3::Normalize(const Vector3& v) noexcept
	{
		return DirectX::XMVector3Normalize(v.vector_);
	}
}