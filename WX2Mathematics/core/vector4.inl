#pragma once

namespace wx2
{
	using namespace DirectX;

	inline Vector4::operator XMFLOAT4() const noexcept
	{
		XMFLOAT4 ret;
		XMStoreFloat4(&ret, vector_);
		return ret;
	}

	inline bool Vector4::operator == (const Vector4& rhs) const noexcept
	{
		return XMVector4Equal(vector_, rhs.vector_);
	}

	inline bool Vector4::operator != (const Vector4& rhs) const noexcept
	{
		return XMVector4NotEqual(vector_, rhs.vector_);
	}

	inline Vector4 Vector4::operator + () const noexcept
	{
		return *this;
	}

	inline Vector4 Vector4::operator - () const noexcept
	{
		return Vector4(XMVectorNegate(vector_));
	}

	inline Vector4 Vector4::operator + (const Vector4& rhs) const noexcept
	{
		return Vector4(XMVectorAdd(vector_, rhs.vector_));
	}

	inline Vector4 Vector4::operator - (const Vector4& rhs) const noexcept
	{
		return Vector4(XMVectorSubtract(vector_, rhs.vector_));
	}

	inline Vector4 Vector4::operator * (const float rhs) const noexcept
	{
		return Vector4(XMVectorScale(vector_, rhs));
	}

	inline Vector4 Vector4::operator / (const float rhs) const noexcept
	{
		WX2_ASSERT_MSG(rhs != 0.0f, "0Ç≈èúéZÇ∑ÇÈÇ±Ç∆ÇÕÇ≈Ç´Ç‹ÇπÇÒÅB");
		return Vector4(XMVectorScale(vector_, 1.0f / rhs));
	}

	inline Vector4& Vector4::operator += (const Vector4& rhs) noexcept
	{
		vector_ = XMVectorAdd(vector_, rhs.vector_);
		return *this;
	}

	inline Vector4& Vector4::operator -= (const Vector4& rhs) noexcept
	{
		vector_ = XMVectorSubtract(vector_, rhs.vector_);
		return *this;
	}

	inline Vector4& Vector4::operator *= (const float rhs) noexcept
	{
		vector_ = XMVectorScale(vector_, rhs);
		return *this;
	}

	inline Vector4& Vector4::operator /= (const float rhs) noexcept
	{
		WX2_ASSERT_MSG(rhs != 0.0f, "0Ç≈èúéZÇ∑ÇÈÇ±Ç∆ÇÕÇ≈Ç´Ç‹ÇπÇÒÅB");
		vector_ = XMVectorScale(vector_, 1.0f / rhs);
		return *this;
	}

	inline void Vector4::Normalized() noexcept
	{
		vector_ = XMVector4Normalize(vector_);
	}

	inline float Vector4::Length() const noexcept
	{
		return XMVectorGetX(XMVector4Length(vector_));
	}

	inline float Vector4::LengthSquared() const noexcept
	{
		return XMVectorGetX(XMVector4LengthSq(vector_));
	}

	inline Vector4 Vector4::Normalize(const Vector4& v) noexcept
	{
		return Vector4(XMVector4Normalize(v.vector_));
	}

	inline float Vector4::Dot(const Vector4& v1, const Vector4& v2) noexcept
	{
		return XMVectorGetX(XMVector4Dot(v1.vector_, v2.vector_));
	}

	inline float Vector4::Distance(const Vector4& v1, const Vector4& v2) noexcept
	{
		return XMVectorGetX(XMVector4Length(XMVectorSubtract(v1.vector_, v2.vector_)));
	}

	inline float Vector4::DistanceSquared(const Vector4& v1, const Vector4& v2) noexcept
	{
		return XMVectorGetX(XMVector4LengthSq(XMVectorSubtract(v1.vector_, v2.vector_)));
	}

	inline Vector4 Vector4::Cross(const Vector4& v1, const Vector4& v2, const Vector4& v3) noexcept
	{
		return Vector4(XMVector4Cross(v1.vector_, v2.vector_, v3.vector_));
	}
}
