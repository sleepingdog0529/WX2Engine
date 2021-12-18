#pragma once
#include "core.h"

namespace wx2
{
#pragma region Scalar

#pragma endregion Scalar

#pragma region Vector2
	inline bool Vector2::operator==(const Vector2& rhs) const noexcept
	{
		return DirectX::XMVector2Equal(vector_, rhs.vector_);
	}

	inline bool Vector2::operator!=(const Vector2& rhs) const noexcept
	{
		return DirectX::XMVector2NotEqual(vector_, rhs.vector_);
	}

	inline Vector2 Vector2::operator-() const noexcept
	{
		return Vector2(DirectX::XMVectorNegate(vector_));
	}

	inline Vector2 Vector2::operator+(const Vector2& rhs) const noexcept
	{
		return Vector2(DirectX::XMVectorAdd(vector_, rhs.vector_));
	}

	inline Vector2 Vector2::operator-(const Vector2& rhs) const noexcept
	{
		return Vector2(DirectX::XMVectorSubtract(vector_, rhs.vector_));
	}

	inline Vector2 Vector2::operator*(const float rhs) const noexcept
	{
		return Vector2(DirectX::XMVectorScale(vector_, rhs));
	}

	inline Vector2 Vector2::operator/(const float rhs) const noexcept
	{
		WX2_ASSERT_MSG(rhs != 0.0f, "0除算が発生しました。");
		return Vector2(DirectX::XMVectorScale(vector_, 1.0f / rhs));
	}

	inline Vector2& Vector2::operator+=(const Vector2& rhs) noexcept
	{
		vector_ = DirectX::XMVectorAdd(vector_, rhs.vector_);
		return *this;
	}

	inline Vector2& Vector2::operator-=(const Vector2& rhs) noexcept
	{
		vector_ = DirectX::XMVectorSubtract(vector_, rhs.vector_);
		return *this;
	}

	inline Vector2& Vector2::operator*=(const float rhs) noexcept
	{
		vector_ = DirectX::XMVectorScale(vector_, rhs);
		return *this;
	}

	inline Vector2& Vector2::operator/=(const float rhs) noexcept
	{
		WX2_ASSERT_MSG(rhs != 0.0f, "0除算が発生しました。");
		vector_ = DirectX::XMVectorScale(vector_, 1.0f / rhs);
		return *this;
	}

	inline const float& Vector2::operator[](const std::size_t index) const noexcept
	{
		WX2_ASSERT_MSG(index < 2, "添え字の値が範囲外です。");
		return vector_.m128_f32[index];
	}

	inline float& Vector2::operator[](const std::size_t index) noexcept
	{
		WX2_ASSERT_MSG(index < 2, "添え字の値が範囲外です。");
		return vector_.m128_f32[index];
	}

	inline Vector2::operator DirectX::XMVECTOR() const noexcept
	{
		return vector_;
	}

	inline Vector2::operator DirectX::XMFLOAT2() const noexcept
	{
		DirectX::XMFLOAT2 xmf;
		DirectX::XMStoreFloat2(&xmf, vector_);
		return xmf;
	}

	inline void Vector2::Normalized() noexcept
	{
		vector_ = DirectX::XMVector2Normalize(vector_);
	}

	inline float Vector2::Length() const noexcept
	{
		return DirectX::XMVectorGetX(DirectX::XMVector2Length(vector_));
	}

	inline float Vector2::LengthSquared() const noexcept
	{
		return DirectX::XMVectorGetX(DirectX::XMVector2LengthSq(vector_));
	}

	inline float Vector2::Angle() const noexcept
	{
		return std::atan2f(vector_.m128_f32[1], vector_.m128_f32[0]);
	}

	inline bool Vector2::InBounds(const Vector2& v, const Vector2& bounds) noexcept
	{
		const auto abs = DirectX::XMVectorAbs(bounds.vector_);
		return DirectX::XMVector2InBounds(v.vector_, abs);
	}

	inline float Vector2::Distance(const Vector2& v1, const Vector2& v2) noexcept
	{
		return DirectX::XMVectorGetX(
				   DirectX::XMVector2Length(
					   DirectX::XMVectorSubtract(v1.vector_, v2.vector_)));
	}

	inline float Vector2::DistanceSquared(const Vector2& v1, const Vector2& v2) noexcept
	{
		return DirectX::XMVectorGetX(
				   DirectX::XMVector2LengthSq(
				       DirectX::XMVectorSubtract(v1.vector_, v2.vector_)));
	}

	inline float Vector2::Dot(const Vector2& v1, const Vector2& v2) noexcept
	{
		return DirectX::XMVectorGetX(DirectX::XMVector2Dot(v1.vector_, v2.vector_));
	}

	inline Vector2 Vector2::Cross(const Vector2& v1, const Vector2& v2) noexcept
	{
		return Vector2(DirectX::XMVector2Cross(v1.vector_, v2.vector_));
	}

	inline Vector2 Vector2::Normalize(const Vector2& v) noexcept
	{
		return Vector2(DirectX::XMVector2Normalize(v.vector_));
	}

	inline Vector2 Vector2::Clamp(const Vector2& v, const float min, const float max) noexcept
	{
		WX2_ASSERT_MSG(min <= max, "最小値に最大値より大きい値が設定されました。");
		return Vector2(DirectX::XMVectorClamp(v.vector_, DirectX::XMVectorReplicate(min), DirectX::XMVectorReplicate(max)));
	}

	inline Vector2 Vector2::Clamp(const Vector2& v, const Vector2& vmin, const Vector2& vmax) noexcept
	{
		return Vector2(DirectX::XMVectorClamp(v.vector_, vmin.vector_, vmax.vector_));
	}

	inline Vector2 Vector2::Min(const Vector2& v1, const Vector2& v2) noexcept
	{
		return Vector2(DirectX::XMVectorMin(v1.vector_, v2.vector_));
	}

	inline Vector2 Vector2::Max(const Vector2& v1, const Vector2& v2) noexcept
	{
		return Vector2(DirectX::XMVectorMax(v1.vector_, v2.vector_));
	}

	inline Vector2 Vector2::Lerp(const Vector2& v1, const Vector2& v2, const float t) noexcept
	{
		return Vector2(DirectX::XMVectorLerp(v1.vector_, v2.vector_, t));
	}

	inline Vector2 Vector2::SmoothStep(const Vector2& v1, const Vector2& v2, const float t) noexcept
	{
		float st = std::clamp(t, 1.0f, 0.0f);
		st = st * st * (3.0f - 2.0f * st);
		return Vector2(DirectX::XMVectorLerp(v1.vector_, v2.vector_, st));
	}

	inline Vector2 Vector2::BaryCentric(
		const Vector2& v1,
		const Vector2& v2, 
		const Vector2& v3,
		const float f,
		const float g) noexcept
	{
		return Vector2(DirectX::XMVectorBaryCentric(v1.vector_, v2.vector_, v3.vector_, f, g));
	}

	inline Vector2 Vector2::CatmullRom(
		const Vector2& v1,
		const Vector2& v2,
		const Vector2& v3, 
		const Vector2& v4,
		const float t) noexcept
	{
		return Vector2(DirectX::XMVectorCatmullRom(v1.vector_, v2.vector_, v3.vector_, v4.vector_, t));
	}

	inline Vector2 Vector2::Hermite(
		const Vector2& v1, 
		const Vector2& t1,
		const Vector2& v2, 
		const Vector2& t2,
		const float t) noexcept
	{
		return Vector2(DirectX::XMVectorHermite(v1.vector_, t1.vector_, v2.vector_, t2.vector_, t));
	}

	inline Vector2 Vector2::Reflect(const Vector2& ivec, const Vector2& nvec) noexcept
	{
		return Vector2(DirectX::XMVector2Reflect(ivec.vector_, nvec.vector_));
	}

	inline Vector2 Vector2::Refract(const Vector2& ivec, const Vector2& nvec, const float refractionIndex) noexcept
	{
		return Vector2(DirectX::XMVector2Refract(ivec.vector_, nvec.vector_, refractionIndex));
	}

	inline Vector2 Vector2::Transform(const Vector2& v, const Quaternion& q) noexcept
	{
		return Vector2(DirectX::XMVector3Rotate(v.vector_, q));
	}

	inline Vector2 Vector2::Transform(const Vector2& v, const Matrix& m) noexcept
	{
		return Vector2(DirectX::XMVector2TransformCoord(v.vector_, m));
	}

	inline Vector2 Vector2::TransformNormal(const Vector2& v, const Matrix& m) noexcept
	{
		return Vector2(DirectX::XMVector2TransformNormal(v.vector_, m));
	}

	inline Vector2 Vector2::FromAngle(const float angle) noexcept
	{
		return { std::cosf(angle), std::sinf(angle) };
	}

	inline std::ostream& operator<<(std::ostream& stream, const Vector2& v)
	{
		stream << std::format("({}, {})", v[0], v[1]);
		return stream;
	}
#pragma endregion Vector2

#pragma region Vector3
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
		return Vector3(DirectX::XMVectorNegate(vector_));
	}

	inline Vector3 Vector3::operator+(const Vector3& rhs) const noexcept
	{
		return Vector3(DirectX::XMVectorAdd(vector_, rhs.vector_));
	}

	inline Vector3 Vector3::operator-(const Vector3& rhs) const noexcept
	{
		return Vector3(DirectX::XMVectorSubtract(vector_, rhs.vector_));
	}

	inline Vector3 Vector3::operator*(const float rhs) const noexcept
	{
		return Vector3(DirectX::XMVectorScale(vector_, rhs));
	}

	inline Vector3 Vector3::operator/(const float rhs) const noexcept
	{
		WX2_ASSERT_MSG(rhs != 0.0f, "0除算が発生しました。");
		return Vector3(DirectX::XMVectorScale(vector_, 1.0f / rhs));
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
		WX2_ASSERT_MSG(rhs != 0.0f, "0除算が発生しました。");
		vector_ = DirectX::XMVectorScale(vector_, 1.0f / rhs);
		return *this;
	}

	inline const float& Vector3::operator[](const std::size_t index) const noexcept
	{
		WX2_ASSERT_MSG(index < 3, "添え字の値が範囲外です。");
		return vector_.m128_f32[index];
	}

	inline float& Vector3::operator[](const std::size_t index) noexcept
	{
		WX2_ASSERT_MSG(index < 3, "添え字の値が範囲外です。");
		return vector_.m128_f32[index];
	}

	inline Vector3::operator DirectX::XMVECTOR() const noexcept
	{
		return vector_;
	}

	inline Vector3::operator DirectX::XMFLOAT3() const noexcept
	{
		DirectX::XMFLOAT3 xmf;
		DirectX::XMStoreFloat3(&xmf, vector_);
		return xmf;
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

	inline bool Vector3::InBounds(const Vector3& v, const Vector3& bounds) noexcept
	{
		const auto abs = DirectX::XMVectorAbs(bounds.vector_);
		return DirectX::XMVector3InBounds(v.vector_, abs);
	}

	inline float Vector3::Distance(const Vector3& v1, const Vector3& v2) noexcept
	{
		return DirectX::XMVectorGetX(
				   DirectX::XMVector3Length(
					   DirectX::XMVectorSubtract(v1.vector_, v2.vector_)));
	}

	inline float Vector3::DistanceSquared(const Vector3& v1, const Vector3& v2) noexcept
	{
		return DirectX::XMVectorGetX(
				   DirectX::XMVector3LengthSq(
					   DirectX::XMVectorSubtract(v1.vector_, v2.vector_)));
	}

	inline float Vector3::Dot(const Vector3& v1, const Vector3& v2) noexcept
	{
		return DirectX::XMVectorGetX(DirectX::XMVector3Dot(v1.vector_, v2.vector_));
	}

	inline Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2) noexcept
	{
		return Vector3(DirectX::XMVector3Cross(v1.vector_, v2.vector_));
	}

	inline Vector3 Vector3::Normalize(const Vector3& v) noexcept
	{
		return Vector3(DirectX::XMVector3Normalize(v.vector_));
	}

	inline Vector3 Vector3::Clamp(const Vector3& v, const float min, const float max) noexcept
	{
		WX2_ASSERT_MSG(min <= max, "最小値に最大値より大きい値が設定されました。");
		return Vector3(DirectX::XMVectorClamp(v.vector_, DirectX::XMVectorReplicate(min), DirectX::XMVectorReplicate(max)));
	}

	inline Vector3 Vector3::Clamp(const Vector3& v, const Vector3& vmin, const Vector3& vmax) noexcept
	{
		return Vector3(DirectX::XMVectorClamp(v.vector_, vmin.vector_, vmax.vector_));
	}

	inline Vector3 Vector3::Min(const Vector3& v1, const Vector3& v2) noexcept
	{
		return Vector3(DirectX::XMVectorMin(v1.vector_, v2.vector_));
	}

	inline Vector3 Vector3::Max(const Vector3& v1, const Vector3& v2) noexcept
	{
		return Vector3(DirectX::XMVectorMax(v1.vector_, v2.vector_));
	}

	inline Vector3 Vector3::Lerp(const Vector3& v1, const Vector3& v2, const float t) noexcept
	{
		return Vector3(DirectX::XMVectorLerp(v1.vector_, v2.vector_, t));
	}

	inline Vector3 Vector3::SmoothStep(const Vector3& v1, const Vector3& v2, float t) noexcept
	{
		float st = std::clamp(t, 0.0f, 1.0f);
		st = st * st * (3.f - 2.f * st);
		return Vector3(DirectX::XMVectorLerp(v1.vector_, v2.vector_, st));
	}

	inline Vector3 Vector3::BaryCentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, const float f, const float g) noexcept
	{
		return Vector3(DirectX::XMVectorBaryCentric(v1.vector_, v2.vector_, v3.vector_, f, g));
	}

	inline Vector3 Vector3::CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t) noexcept
	{
		return Vector3(DirectX::XMVectorCatmullRom(v1.vector_, v2.vector_, v3.vector_, v4.vector_, t));
	}

	inline Vector3 Vector3::Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t) noexcept
	{
		return Vector3(DirectX::XMVectorHermite(v1.vector_, t1.vector_, v2.vector_, t2.vector_, t));
	}

	inline Vector3 Vector3::Reflect(const Vector3& ivec, const Vector3& nvec) noexcept
	{
		return Vector3(DirectX::XMVector3Reflect(ivec.vector_, nvec.vector_));
	}

	inline Vector3 Vector3::Refract(const Vector3& ivec, const Vector3& nvec, const float refractionIndex) noexcept
	{
		return Vector3(DirectX::XMVector3Refract(ivec.vector_, nvec.vector_, refractionIndex));
	}

	inline Vector3 Vector3::Transform(const Vector3& v, const Quaternion& q) noexcept
	{
		return Vector3(DirectX::XMVector3Rotate(v.vector_, q));
	}

	inline Vector3 Vector3::Transform(const Vector3& v, const Matrix& m) noexcept
	{
		return Vector3(DirectX::XMVector3TransformCoord(v.vector_, m));
	}

	inline Vector3 Vector3::TransformNormal(const Vector3& v, const Matrix& m) noexcept
	{
		return Vector3(DirectX::XMVector3TransformNormal(v.vector_, m));
	}
#pragma endregion Vector3

#pragma region Vector4
	inline bool Vector4::operator==(const Vector4& rhs) const noexcept
	{
		return DirectX::XMVector4Equal(vector_, rhs.vector_);
	}

	inline bool Vector4::operator!=(const Vector4& rhs) const noexcept
	{
		return DirectX::XMVector4NotEqual(vector_, rhs.vector_);
	}

	inline Vector4 Vector4::operator-() const noexcept
	{
		return Vector4(DirectX::XMVectorNegate(vector_));
	}

	inline Vector4 Vector4::operator+(const Vector4& rhs) const noexcept
	{
		return Vector4(DirectX::XMVectorAdd(vector_, rhs.vector_));
	}

	inline Vector4 Vector4::operator-(const Vector4& rhs) const noexcept
	{
		return Vector4(DirectX::XMVectorSubtract(vector_, rhs.vector_));
	}

	inline Vector4 Vector4::operator*(const float rhs) const noexcept
	{
		return Vector4(DirectX::XMVectorScale(vector_, rhs));
	}

	inline Vector4 Vector4::operator/(const float rhs) const noexcept
	{
		WX2_ASSERT_MSG(rhs != 0.0f, "0除算が発生しました。");
		return Vector4(DirectX::XMVectorScale(vector_, 1.0f / rhs));
	}

	inline Vector4& Vector4::operator+=(const Vector4& rhs) noexcept
	{
		vector_ = DirectX::XMVectorAdd(vector_, rhs.vector_);
		return *this;
	}

	inline Vector4& Vector4::operator-=(const Vector4& rhs) noexcept
	{
		vector_ = DirectX::XMVectorSubtract(vector_, rhs.vector_);
		return *this;
	}

	inline Vector4& Vector4::operator*=(const float rhs) noexcept
	{
		vector_ = DirectX::XMVectorScale(vector_, rhs);
		return *this;
	}

	inline Vector4& Vector4::operator/=(const float rhs) noexcept
	{
		WX2_ASSERT_MSG(rhs != 0.0f, "0除算が発生しました。");
		vector_ = DirectX::XMVectorScale(vector_, 1.0f / rhs);
		return *this;
	}

	inline const float& Vector4::operator[](const std::size_t index) const noexcept
	{
		WX2_ASSERT_MSG(index < 4, "添え字の値が範囲外です。");
		return vector_.m128_f32[index];
	}

	inline float& Vector4::operator[](const std::size_t index) noexcept
	{
		WX2_ASSERT_MSG(index < 4, "添え字の値が範囲外です。");
		return vector_.m128_f32[index];
	}

	inline Vector4::operator DirectX::XMVECTOR() const noexcept
	{
		return vector_;
	}

	inline Vector4::operator DirectX::XMFLOAT4() const noexcept
	{
		DirectX::XMFLOAT4 xmf;
		DirectX::XMStoreFloat4(&xmf, vector_);
		return xmf;
	}

	inline void Vector4::Normalized() noexcept
	{
		vector_ = DirectX::XMVector4Normalize(vector_);
	}

	inline float Vector4::Length() const noexcept
	{
		return DirectX::XMVectorGetX(DirectX::XMVector4Length(vector_));
	}

	inline float Vector4::LengthSquared() const noexcept
	{
		return DirectX::XMVectorGetX(DirectX::XMVector4LengthSq(vector_));
	}

	inline bool Vector4::InBounds(const Vector4& v, const Vector4& bounds) noexcept
	{
		const auto abs = DirectX::XMVectorAbs(bounds.vector_);
		return DirectX::XMVector4InBounds(v.vector_, abs);
	}

	inline float Vector4::Distance(const Vector4& v1, const Vector4& v2) noexcept
	{
		return DirectX::XMVectorGetX(
				   DirectX::XMVector4Length(
					   DirectX::XMVectorSubtract(v1.vector_, v2.vector_)));
	}

	inline float Vector4::DistanceSquared(const Vector4& v1, const Vector4& v2) noexcept
	{
		return DirectX::XMVectorGetX(
				   DirectX::XMVector4LengthSq(
					   DirectX::XMVectorSubtract(v1.vector_, v2.vector_)));
	}

	inline float Vector4::Dot(const Vector4& v1, const Vector4& v2) noexcept
	{
		return DirectX::XMVectorGetX(DirectX::XMVector4Dot(v1.vector_, v2.vector_));
	}

	inline Vector4 Vector4::Cross(const Vector4& v1, const Vector4& v2, const Vector4& v3) noexcept
	{
		return Vector4(DirectX::XMVector4Cross(v1.vector_, v2.vector_, v3.vector_));
	}

	inline Vector4 Vector4::Normalize(const Vector4& v) noexcept
	{
		return Vector4(DirectX::XMVector4Normalize(v.vector_));
	}

	inline Vector4 Vector4::Clamp(const Vector4& v, const float min, const float max) noexcept
	{
		WX2_ASSERT_MSG(min <= max, "最小値に最大値より大きい値が設定されました。");
		return Vector4(DirectX::XMVectorClamp(v.vector_, DirectX::XMVectorReplicate(min), DirectX::XMVectorReplicate(max)));
	}

	inline Vector4 Vector4::Clamp(const Vector4& v, const Vector4& vmin, const Vector4& vmax) noexcept
	{
		return Vector4(DirectX::XMVectorClamp(v.vector_, vmin.vector_, vmax.vector_));
	}

	inline Vector4 Vector4::Min(const Vector4& v1, const Vector4& v2) noexcept
	{
		return Vector4(DirectX::XMVectorMin(v1.vector_, v2.vector_));
	}

	inline Vector4 Vector4::Max(const Vector4& v1, const Vector4& v2) noexcept
	{
		return Vector4(DirectX::XMVectorMax(v1.vector_, v2.vector_));
	}

	inline Vector4 Vector4::Lerp(const Vector4& v1, const Vector4& v2, const float t) noexcept
	{
		return Vector4(DirectX::XMVectorLerp(v1.vector_, v2.vector_, t));
	}

	inline Vector4 Vector4::SmoothStep(const Vector4& v1, const Vector4& v2, const float t) noexcept
	{
		float st = std::clamp(t, 0.0f, 1.0f);
		st = st * st * (3.f - 2.f * st);
		return Vector4(DirectX::XMVectorLerp(v1.vector_, v2.vector_, st));
	}

	inline Vector4 Vector4::BaryCentric(
		const Vector4& v1, 
		const Vector4& v2, 
		const Vector4& v3,
		const float f,
		const float g) noexcept
	{
		return Vector4(DirectX::XMVectorBaryCentric(v1.vector_, v2.vector_, v3.vector_, f, g));
	}

	inline Vector4 Vector4::CatmullRom(
		const Vector4& v1, 
		const Vector4& v2, 
		const Vector4& v3, 
		const Vector4& v4,
		const float t) noexcept
	{
		return Vector4(DirectX::XMVectorCatmullRom(v1.vector_, v2.vector_, v3.vector_, v4.vector_, t));
	}

	inline Vector4 Vector4::Hermite(
		const Vector4& v1, 
		const Vector4& t1, 
		const Vector4& v2, 
		const Vector4& t2,
		const float t) noexcept
	{
		return Vector4(DirectX::XMVectorHermite(v1.vector_, t1.vector_, v2.vector_, t2.vector_, t));
	}

	inline Vector4 Vector4::Reflect(const Vector4& ivec, const Vector4& nvec) noexcept
	{
		return Vector4(DirectX::XMVector4Reflect(ivec.vector_, nvec.vector_));
	}

	inline Vector4 Vector4::Refract(const Vector4& ivec, const Vector4& nvec, const float refractionIndex) noexcept
	{
		return Vector4(DirectX::XMVector4Refract(ivec.vector_, nvec.vector_, refractionIndex));
	}

	inline Vector4 Vector4::Transform(const Vector2& v, const Quaternion& q) noexcept
	{
		return Vector4(DirectX::XMVectorSelect(v, DirectX::XMVector3Rotate(v, q), DirectX::g_XMSelect1110));
	}

	inline Vector4 Vector4::Transform(const Vector3& v, const Quaternion& q) noexcept
	{
		return Vector4(DirectX::XMVectorSelect(v, DirectX::XMVector3Rotate(v, q), DirectX::g_XMSelect1110));
	}

	inline Vector4 Vector4::Transform(const Vector4& v, const Quaternion& q) noexcept
	{
		return Vector4(DirectX::XMVectorSelect(v.vector_, DirectX::XMVector3Rotate(v.vector_, q), DirectX::g_XMSelect1110));
	}

	inline Vector4 Vector4::Transform(const Vector4& v, const Matrix& m) noexcept
	{
		return Vector4(DirectX::XMVector4Transform(v.vector_, m));
	}
#pragma endregion Vector4

#pragma region Matrix
	constexpr Matrix::Matrix() noexcept
		: matrix_{
			{1.0f, 0.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f} }
	{
	}

	constexpr Matrix::Matrix(
		const float m00, const float m01, const float m02, const float m03,
		const float m10, const float m11, const float m12, const float m13,
		const float m20, const float m21, const float m22, const float m23,
		const float m30, const float m31, const float m32, const float m33) noexcept
		: matrix_{
			{m00, m01, m02, m03},
			{m10, m11, m12, m13},
			{m20, m21, m22, m23},
			{m30, m31, m32, m33} }
	{
	}

	constexpr Matrix::Matrix(
		const DirectX::XMVECTOR& r0,
		const DirectX::XMVECTOR& r1,
		const DirectX::XMVECTOR& r2,
		const DirectX::XMVECTOR& r3) noexcept
		: matrix_(r0, r1, r2, r3)
	{
	}

	constexpr Matrix::Matrix(
		const Vector4& r0,
		const Vector4& r1,
		const Vector4& r2,
		const Vector4& r3) noexcept
		: matrix_(r0, r1, r2, r3)
	{
	}

	inline bool Matrix::operator==(const Matrix& rhs) const noexcept
	{
		return
			DirectX::XMVector4Equal(matrix_.r[0], rhs.matrix_.r[0]) &&
			DirectX::XMVector4Equal(matrix_.r[1], rhs.matrix_.r[1]) &&
			DirectX::XMVector4Equal(matrix_.r[2], rhs.matrix_.r[2]) &&
			DirectX::XMVector4Equal(matrix_.r[3], rhs.matrix_.r[3]);
	}

	inline bool Matrix::operator!=(const Matrix& rhs) const noexcept
	{
		return
			DirectX::XMVector4NotEqual(matrix_.r[0], rhs.matrix_.r[0]) ||
			DirectX::XMVector4NotEqual(matrix_.r[1], rhs.matrix_.r[1]) ||
			DirectX::XMVector4NotEqual(matrix_.r[2], rhs.matrix_.r[2]) ||
			DirectX::XMVector4NotEqual(matrix_.r[3], rhs.matrix_.r[3]);
	}

	inline Matrix Matrix::operator*(const Matrix& rhs) const noexcept
	{
		return Matrix(DirectX::XMMatrixMultiply(matrix_, rhs.matrix_));
	}

	inline Matrix& Matrix::operator*=(const Matrix& rhs) noexcept
	{
		matrix_ = DirectX::XMMatrixMultiply(matrix_, rhs.matrix_);
		return *this;
	}

	inline const Vector4& Matrix::operator[](const std::size_t index) const noexcept
	{
		WX2_ASSERT_MSG(index < 4, "添え字の値が範囲外です。");
		return Vector4(matrix_.r[index]);
	}

	inline Vector4& Matrix::operator[](const std::size_t index) noexcept
	{
		WX2_ASSERT_MSG(index < 4, "添え字の値が範囲外です。");
		return reinterpret_cast<Vector4&>(matrix_.r[index]);
	}

	inline Matrix::operator DirectX::XMMATRIX() const noexcept
	{
		return matrix_;
	}

	inline Matrix::operator DirectX::XMFLOAT4X4() const noexcept
	{
		DirectX::XMFLOAT4X4 xmf;
		DirectX::XMStoreFloat4x4(&xmf, matrix_);
		return xmf;
	}

	inline Vector3 Matrix::Right() const noexcept
	{
		return Vector3::Normalize(Vector3(
			matrix_.r[0].m128_f32[0],
			matrix_.r[0].m128_f32[1],
			matrix_.r[0].m128_f32[2]));
	}

	inline Vector3 Matrix::Left() const noexcept
	{
		return Vector3::Normalize(Vector3(
			-matrix_.r[0].m128_f32[0],
			-matrix_.r[0].m128_f32[1],
			-matrix_.r[0].m128_f32[2]));
	}

	inline Vector3 Matrix::Up() const noexcept
	{
		return Vector3::Normalize(Vector3(
			matrix_.r[1].m128_f32[0],
			matrix_.r[1].m128_f32[1],
			matrix_.r[1].m128_f32[2]));
	}

	inline Vector3 Matrix::Down() const noexcept
	{
		return Vector3::Normalize(Vector3(
			-matrix_.r[1].m128_f32[0],
			-matrix_.r[1].m128_f32[1],
			-matrix_.r[1].m128_f32[2]));
	}

	inline Vector3 Matrix::Forward() const noexcept
	{
		return Vector3::Normalize(Vector3(
			matrix_.r[2].m128_f32[0],
			matrix_.r[2].m128_f32[1],
			matrix_.r[2].m128_f32[2]));
	}

	inline Vector3 Matrix::Backward() const noexcept
	{
		return Vector3::Normalize(Vector3(
			-matrix_.r[2].m128_f32[0],
			-matrix_.r[2].m128_f32[1],
			-matrix_.r[2].m128_f32[2]));
	}

	inline float Matrix::Determinant() const noexcept
	{
		return DirectX::XMVectorGetX(DirectX::XMMatrixDeterminant(matrix_));
	}

	inline Matrix Matrix::Billboard(
		const Vector3& object, 
		const Vector3& cameraPosition,
		const Vector3& cameraUp, 
		const Vector3* cameraForward) noexcept
	{
		DirectX::XMVECTOR z = DirectX::XMVectorSubtract(object, cameraPosition);

		if (DirectX::XMVector3Less(DirectX::XMVector3LengthSq(z), DirectX::g_XMEpsilon))
		{
			if (cameraForward)
			{
				z = DirectX::XMVectorNegate(*cameraForward);
			}
			else
			{
				z = DirectX::g_XMNegIdentityR2;
			}
		}
		else
		{
			z = DirectX::XMVector3Normalize(z);
		}

		const DirectX::XMVECTOR x = 
			DirectX::XMVector3Normalize(DirectX::XMVector3Cross(cameraUp, z));

		const DirectX::XMVECTOR y = DirectX::XMVector3Cross(z, x);

		return { x, y, z, DirectX::XMVectorSetW(object, 1.0f) };
	}

	inline Matrix Matrix::CreateConstrainedBillboard(
		const Vector3& object, 
		const Vector3& cameraPosition,
		const Vector3& rotateAxis,
		const Vector3* cameraForward, 
		const Vector3* objectForward) noexcept
	{
		// 1.0 - XMConvertToRadians( 0.1f );
		static constexpr DirectX::XMVECTORF32 MIN_ANGLE
			= { { { 0.99825467075f, 0.99825467075f, 0.99825467075f, 0.99825467075f } } }; 

		DirectX::XMVECTOR faceDir = DirectX::XMVectorSubtract(object, cameraPosition);

		if (DirectX::XMVector3Less(DirectX::XMVector3LengthSq(faceDir), DirectX::g_XMEpsilon))
		{
			if (cameraForward)
			{
				faceDir = DirectX::XMVectorNegate(*cameraForward);
			}
			else
			{
				faceDir = DirectX::g_XMNegIdentityR2;
			}
		}
		else
		{
			faceDir = DirectX::XMVector3Normalize(faceDir);
		}

		DirectX::XMVECTOR x, z;
		DirectX::XMVECTOR dot = DirectX::XMVectorAbs(DirectX::XMVector3Dot(rotateAxis, faceDir));

		if (DirectX::XMVector3Greater(dot, MIN_ANGLE))
		{
			if (objectForward)
			{
				z = *objectForward;
				dot = DirectX::XMVectorAbs(DirectX::XMVector3Dot(rotateAxis, z));
				if (DirectX::XMVector3Greater(dot, MIN_ANGLE))
				{
					dot = DirectX::XMVectorAbs(DirectX::XMVector3Dot(rotateAxis, DirectX::g_XMNegIdentityR2));
					z = (DirectX::XMVector3Greater(dot, MIN_ANGLE)) ? DirectX::g_XMIdentityR0 : DirectX::g_XMNegIdentityR2;
				}
			}
			else
			{
				dot = DirectX::XMVectorAbs(DirectX::XMVector3Dot(rotateAxis, DirectX::g_XMNegIdentityR2));
				z = (DirectX::XMVector3Greater(dot, MIN_ANGLE)) ? DirectX::g_XMIdentityR0 : DirectX::g_XMNegIdentityR2;
			}

			x = DirectX::XMVector3Cross(rotateAxis, z);
			x = DirectX::XMVector3Normalize(x);

			z = DirectX::XMVector3Cross(x, rotateAxis);
			z = DirectX::XMVector3Normalize(z);
		}
		else
		{
			x = DirectX::XMVector3Cross(rotateAxis, faceDir);
			x = DirectX::XMVector3Normalize(x);

			z = DirectX::XMVector3Cross(x, rotateAxis);
			z = DirectX::XMVector3Normalize(z);
		}

		return { x, rotateAxis, z, DirectX::XMVectorSetW(object, 1.0f) };
	}


	inline Matrix Matrix::Transpose(const Matrix& m) noexcept
	{
		return Matrix(DirectX::XMMatrixTranspose(m.matrix_));
	}

	inline Matrix Matrix::Inverse(const Matrix& m) noexcept
	{
		return Matrix(DirectX::XMMatrixInverse(nullptr, m.matrix_));
	}

	inline Matrix Matrix::Translation(const Vector3& position) noexcept
	{
		return Matrix(DirectX::XMMatrixTranslationFromVector(position));
	}

	inline Matrix Matrix::Translation(const float x, const float y, const float z) noexcept
	{
		return Matrix(DirectX::XMMatrixTranslation(x, y, z));
	}

	inline Matrix Matrix::Scale(const Vector3& scales) noexcept
	{
		return Matrix(DirectX::XMMatrixScalingFromVector(scales));
	}

	inline Matrix Matrix::Scale(const float xs, const float ys, const float zs) noexcept
	{
		return Matrix(DirectX::XMMatrixScaling(xs, ys, zs));
	}

	inline Matrix Matrix::Scale(const float scale) noexcept
	{
		return Matrix(DirectX::XMMatrixScaling(scale, scale, scale));
	}

	inline Matrix Matrix::RotationX(const float rotation) noexcept
	{
		return Matrix(DirectX::XMMatrixRotationX(rotation));
	}

	inline Matrix Matrix::RotationY(const float rotation) noexcept
	{
		return Matrix(DirectX::XMMatrixRotationY(rotation));
	}

	inline Matrix Matrix::RotationZ(const float rotation) noexcept
	{
		return Matrix(DirectX::XMMatrixRotationZ(rotation));
	}

	inline Matrix Matrix::RotationAxisAngle(const Vector3& axis, const float radians) noexcept
	{
		return Matrix(DirectX::XMMatrixRotationAxis(axis, radians));
	}

	inline Matrix Matrix::RotationFromQuaternion(const Quaternion& q) noexcept
	{
		return Matrix(DirectX::XMMatrixRotationQuaternion(q));
	}

	inline Matrix Matrix::RotationYawPitchRoll(const float yaw, const float pitch, const float roll) noexcept
	{
		return Matrix(DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
	}

	inline Matrix Matrix::PerspectiveFieldOfView(
		const float fov,
		const float aspectRatio,
		const float nearPlane,
		const float farPlane) noexcept
	{
		return Matrix(DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane));
	}

	inline Matrix Matrix::Perspective(const float width, const float height, const float nearPlane, const float farPlane) noexcept
	{
		return Matrix(DirectX::XMMatrixPerspectiveLH(width, height, nearPlane, farPlane));
	}

	inline Matrix Matrix::PerspectiveOffCenter(
		const float left,
		const float right,
		const float bottom,
		const float top,
		const float nearPlane,
		const float farPlane) noexcept
	{
		return Matrix(DirectX::XMMatrixPerspectiveOffCenterLH(left, right, bottom, top, nearPlane, farPlane));
	}

	inline Matrix Matrix::Orthographic(
		const float width,
		const float height,
		const float zNearPlane,
		const float zFarPlane) noexcept
	{
		return Matrix(DirectX::XMMatrixOrthographicLH(width, height, zNearPlane, zFarPlane));
	}

	inline Matrix Matrix::OrthographicOffCenter(
		const float left,
		const float right,
		const float bottom,
		const float top,
		const float zNearPlane,
		const float zFarPlane) noexcept
	{
		return Matrix(DirectX::XMMatrixOrthographicOffCenterLH(left, right, bottom, top, zNearPlane, zFarPlane));
	}

	inline Matrix Matrix::LookAt(const Vector3& position, const Vector3& target, const Vector3& up) noexcept
	{
		return Matrix(DirectX::XMMatrixLookAtLH(position, target, up));
	}

	inline Matrix Matrix::World(const Vector3& position, const Vector3& forward, const Vector3& up) noexcept
	{
		Vector3 zaxis = Vector3::Normalize(-forward);
		Vector3 xaxis = Vector3::Normalize(Vector3::Cross(Vector3::Normalize(up), zaxis));
		Vector3 yaxis = Vector3::Cross(zaxis, xaxis);

		return {
			xaxis[0], xaxis[1], xaxis[2], 0.0f,
			yaxis[0], yaxis[1], yaxis[2], 0.0f,
			zaxis[0], zaxis[1], zaxis[2], 0.0f,
			position[0], position[1], position[2], 1.0f };
	}

	inline Matrix Matrix::Lerp(const Matrix& m1, const Matrix& m2, const float t) noexcept
	{
		return {
			Vector4(DirectX::XMVectorLerp(m1[0], m2[0], t)),
			Vector4(DirectX::XMVectorLerp(m1[1], m2[1], t)),
			Vector4(DirectX::XMVectorLerp(m1[2], m2[2], t)),
			Vector4(DirectX::XMVectorLerp(m1[3], m2[3], t)) };
	}

	inline Matrix Matrix::Transform(const Matrix& m, const Quaternion& rotation) noexcept
	{
		return Matrix(DirectX::XMMatrixMultiply(m, DirectX::XMMatrixRotationQuaternion(rotation)));
	}

	inline constexpr Matrix Matrix::Identity() noexcept
	{
		return {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };
	}
#pragma endregion Matrix

#pragma region Quaternion
	inline bool Quaternion::operator ==(const Quaternion& rhs) const noexcept
	{
		return DirectX::XMQuaternionEqual(quaternion_, rhs.quaternion_);
	}

	inline bool Quaternion::operator!=(const Quaternion& rhs) const noexcept
	{
		return DirectX::XMQuaternionNotEqual(quaternion_, rhs.quaternion_);
	}

	inline Quaternion Quaternion::operator + (const Quaternion& rhs) const noexcept
	{
		return Quaternion(DirectX::XMVectorAdd(quaternion_, rhs.quaternion_));
	}

	inline Quaternion Quaternion::operator - (const Quaternion& rhs) const noexcept
	{
		return Quaternion(DirectX::XMVectorSubtract(quaternion_, rhs.quaternion_));
	}

	inline Quaternion Quaternion::operator * (const Quaternion& rhs) const noexcept
	{
		return Quaternion(DirectX::XMQuaternionMultiply(quaternion_, rhs.quaternion_));
	}

	inline Quaternion Quaternion::operator * (const float rhs) const noexcept
	{
		return Quaternion(DirectX::XMVectorScale(quaternion_, rhs));
	}

	inline Quaternion& Quaternion::operator += (const Quaternion& rhs) noexcept
	{
		quaternion_ = DirectX::XMVectorAdd(quaternion_, rhs.quaternion_);
		return *this;
	}

	inline Quaternion& Quaternion::operator -= (const Quaternion& rhs) noexcept
	{
		quaternion_ = DirectX::XMVectorSubtract(quaternion_, rhs.quaternion_);
		return *this;
	}

	inline Quaternion& Quaternion::operator *= (const Quaternion& rhs) noexcept
	{
		quaternion_ = DirectX::XMQuaternionMultiply(quaternion_, rhs.quaternion_);
		return *this;
	}

	inline Quaternion& Quaternion::operator *= (const float rhs) noexcept
	{
		quaternion_ = DirectX::XMVectorScale(quaternion_, rhs);
		return *this;
	}

	inline Quaternion::operator DirectX::XMVECTOR() const noexcept
	{
		return quaternion_;
	}

	inline void Quaternion::Normalized() noexcept
	{
		quaternion_ = DirectX::XMQuaternionNormalize(quaternion_);
	}

	inline float Quaternion::Length() const noexcept
	{
		return DirectX::XMVectorGetX(DirectX::XMQuaternionLength(quaternion_));
	}

	inline float Quaternion::LengthSquared() const noexcept
	{
		return DirectX::XMVectorGetX(DirectX::XMQuaternionLengthSq(quaternion_));
	}

	inline Vector3 Quaternion::Forward() const noexcept
	{
		const auto [x, y, z, w] = quaternion_.m128_f32;

		return {
			2 * (x * z + w * y),
			2 * (y * z - w * x),
			1 - 2 * (x * x + y * y) };
	}

	inline Vector3 Quaternion::Up() const noexcept
	{
		const auto [x, y, z, w] = quaternion_.m128_f32;

		return {
			2 * (x * y - w * z),
			1 - 2 * (x * x + z * z),
			2 * (y * z + w * x) };
	}

	inline Vector3 Quaternion::Right() const noexcept
	{
		const auto [x, y, z, w] = quaternion_.m128_f32;

		return {
			1 - 2 * (y * y + z * z),
			2 * (x * y + w * z),
			2 * (x * z - w * y) };
	}

	inline float Quaternion::Dot(const Quaternion& q1, const Quaternion& q2) noexcept
	{
		return DirectX::XMVectorGetX(DirectX::XMQuaternionDot(q1.quaternion_, q2.quaternion_));
	}

	inline Quaternion Quaternion::Normalize(const Quaternion& q) noexcept
	{
		return Quaternion(DirectX::XMQuaternionNormalize(q.quaternion_));
	}

	inline Quaternion Quaternion::Conjugate(const Quaternion& q) noexcept
	{
		return Quaternion(DirectX::XMQuaternionConjugate(q.quaternion_));
	}

	inline Quaternion Quaternion::Inverse(const Quaternion& q) noexcept
	{
		return Quaternion(DirectX::XMQuaternionInverse(q.quaternion_));
	}

	inline Quaternion Quaternion::RotationX(const float rotation) noexcept
	{
		return Quaternion(DirectX::XMQuaternionRotationRollPitchYaw(rotation, 0.0f, 0.0f));
	}

	inline Quaternion Quaternion::RotationY(const float rotation) noexcept
	{
		return Quaternion(DirectX::XMQuaternionRotationRollPitchYaw(0.0f, rotation, 0.0f));
	}

	inline Quaternion Quaternion::RotationZ(const float rotation) noexcept
	{
		return Quaternion(DirectX::XMQuaternionRotationRollPitchYaw(0.0f, 0.0f, rotation));
	}

	inline Quaternion Quaternion::AxisAngle(const Vector3& axis, const float angle) noexcept
	{
		return Quaternion(DirectX::XMQuaternionRotationAxis(axis, angle));
	}

	inline Quaternion Quaternion::YawPitchRoll(const float yaw, const float pitch, const float roll) noexcept
	{
		return Quaternion(DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
	}

	inline Quaternion Quaternion::FromRotationMatrix(const Matrix& m) noexcept
	{
		return Quaternion(DirectX::XMQuaternionRotationMatrix(m));
	}

	inline Quaternion Quaternion::Lerp(const Quaternion& q1, const Quaternion& q2, float t) noexcept
	{
		const DirectX::XMVECTOR dot = DirectX::XMVector4Dot(q1, q2);

		if (DirectX::XMVector4GreaterOrEqual(dot, DirectX::XMVectorZero()))
		{
			return Quaternion(
				DirectX::XMQuaternionNormalize(
					DirectX::XMVectorLerp(q1.quaternion_, q2.quaternion_, t)));
		}

		const DirectX::XMVECTOR x0 =
			DirectX::XMVectorMultiply(q1.quaternion_, DirectX::XMVectorReplicate(1.0f - t));
		const DirectX::XMVECTOR x1 = 
			DirectX::XMVectorMultiply(q2.quaternion_, DirectX::XMVectorReplicate(t));

		return Quaternion(
			DirectX::XMQuaternionNormalize(
				DirectX::XMVectorSubtract(x0, x1)));
	}

	inline Quaternion Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, float t) noexcept
	{
		return Quaternion(DirectX::XMQuaternionSlerp(q1.quaternion_, q2.quaternion_, t));
	}

	inline Quaternion Quaternion::Concatenate(const Quaternion& q1, const Quaternion& q2) noexcept
	{
		return Quaternion(DirectX::XMQuaternionMultiply(q1.quaternion_, q2.quaternion_));
	}

	inline bool Color::operator == (const Color& rhs) const noexcept
	{
		return DirectX::XMColorEqual(color_, rhs.color_);
	}

	inline bool Color::operator != (const Color& rhs) const noexcept
	{
		return DirectX::XMColorNotEqual(color_, rhs.color_);
	}

	inline Color Color::operator - () const noexcept
	{
		return Color(DirectX::XMVectorNegate(color_));
	}

	inline Color Color::operator + (const Color& rhs) const noexcept
	{
		return Color(DirectX::XMVectorAdd(color_, rhs.color_));
	}

	inline Color Color::operator - (const Color& rhs) const noexcept
	{
		return Color(DirectX::XMVectorSubtract(color_, rhs.color_));
	}

	inline Color Color::operator * (const float rhs) const noexcept
	{
		return Color(DirectX::XMVectorScale(color_, rhs));
	}

	inline Color Color::operator / (const float rhs) const noexcept
	{
		WX2_ASSERT_MSG(rhs != 0.0f, "0除算が発生しました。");
		return Color(DirectX::XMVectorScale(color_, 1.0f / rhs));
	}

	inline Color& Color::operator += (const Color& rhs) noexcept
	{
		color_ = DirectX::XMVectorAdd(color_, rhs.color_);
		return *this;
	}

	inline Color& Color::operator -= (const Color& rhs) noexcept
	{
		color_ = DirectX::XMVectorSubtract(color_, rhs.color_);
		return *this;
	}

	inline Color& Color::operator *= (const float rhs) noexcept
	{
		color_ = DirectX::XMVectorScale(color_, rhs);
		return *this;
	}

	inline Color& Color::operator /= (const float rhs) noexcept
	{
		WX2_ASSERT_MSG(rhs != 0.0f, "0除算が発生しました。");
		color_ = DirectX::XMVectorScale(color_, 1.0f / rhs);
		return *this;
	}

	inline const float& Color::operator[] (const std::size_t index) const noexcept
	{
		WX2_ASSERT_MSG(index < 4, "添え字の値が範囲外です。");
		return color_.m128_f32[index];
	}

	inline float& Color::operator[] (const std::size_t index) noexcept
	{
		WX2_ASSERT_MSG(index < 4, "添え字の値が範囲外です。");
		return color_.m128_f32[index];
	}

	inline Color::operator DirectX::XMVECTOR() const noexcept
	{
		return color_;
	}

	inline Color::operator DirectX::XMFLOAT4() const noexcept
	{
		DirectX::XMFLOAT4 xmf;
		DirectX::XMStoreFloat4(&xmf, color_);
		return xmf;
	}

	inline Color Color::Negate(const Color& c) noexcept
	{
		return Color(DirectX::XMColorNegative(c.color_));
	}

	inline Color Color::Saturate(const Color& c) noexcept
	{
		return Color(DirectX::XMVectorSaturate(c.color_));
	}

	inline Color Color::Premultiply(const Color& c) noexcept
	{
		const DirectX::XMVECTOR a = DirectX::XMVectorSetW(DirectX::XMVectorSplatW(c.color_), 1.0f);
		return Color(DirectX::XMVectorMultiply(c.color_, a));
	}

	inline Color Color::AdjustSaturation(const Color& c, const float sat) noexcept
	{
		return Color(DirectX::XMColorAdjustSaturation(c.color_, sat));
	}

	inline Color Color::AdjustContrast(const Color& c, const float contrast) noexcept
	{
		return Color(DirectX::XMColorAdjustContrast(c.color_, contrast));
	}

	inline Color Color::Modulate(const Color& c1, const Color& c2) noexcept
	{
		return Color(DirectX::XMColorModulate(c1.color_, c2.color_));
	}

	inline Color Color::Lerp(const Color& c1, const Color& c2, const float t) noexcept
	{
		return Color(DirectX::XMVectorLerp(c1.color_, c2.color_, t));
	}

	inline Color Color::FromHSL(const float h, const float s, const float l, const float a) noexcept
	{
		return Color(DirectX::XMColorHSLToRGB(DirectX::XMVectorSet(h, s, l, a)));
	}

	inline Color Color::FromHSV(const float h, const float s, const float v, const float a) noexcept
	{
		return Color(DirectX::XMColorHSVToRGB(DirectX::XMVectorSet(h, s, v, a)));
	}
#pragma endregion Quaternion
}