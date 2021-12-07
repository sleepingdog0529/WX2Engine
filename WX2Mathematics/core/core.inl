#pragma once
#include "core.h"
#include <WX2Common.h>

namespace wx2
{
#pragma region Scalar

#pragma endregion Scalar

#pragma region Vector2

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

	inline Vector3 Normalize(const Vector3& v) noexcept
	{
		return Vector3(DirectX::XMVector3Normalize(v));
	}

	inline float Dot(const Vector3& v1, const Vector3& v2) noexcept
	{
		return DirectX::XMVectorGetX(DirectX::XMVector3Dot(v1, v2));
	}

	inline Vector3 Cross(const Vector3& v1, const Vector3& v2) noexcept
	{
		return Vector3(DirectX::XMVector3Cross(v1, v2));
	}
#pragma endregion Vector3

#pragma region Vector2
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
#pragma endregion Vector2

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

	inline constexpr Vector3 Matrix::Right() const noexcept
	{
		return Normalize(Vector3(
			matrix_.r[0].m128_f32[0],
			matrix_.r[0].m128_f32[1],
			matrix_.r[0].m128_f32[2]));
	}

	inline constexpr Vector3 Matrix::Left() const noexcept
	{
		return {
			-matrix_.r[0].m128_f32[0],
			-matrix_.r[0].m128_f32[1],
			-matrix_.r[0].m128_f32[2] };
	}

	inline constexpr Vector3 Matrix::Up() const noexcept
	{
		return {
			matrix_.r[1].m128_f32[0],
			matrix_.r[1].m128_f32[1],
			matrix_.r[1].m128_f32[2] };
	}

	inline constexpr Vector3 Matrix::Down() const noexcept
	{
		return {
			-matrix_.r[1].m128_f32[0],
			-matrix_.r[1].m128_f32[1],
			-matrix_.r[1].m128_f32[2] };
	}

	inline constexpr Vector3 Matrix::Forward() const noexcept
	{
		return {
			matrix_.r[2].m128_f32[0],
			matrix_.r[2].m128_f32[1],
			matrix_.r[2].m128_f32[2] };
	}

	inline constexpr Vector3 Matrix::Backward() const noexcept
	{
		return {
			-matrix_.r[2].m128_f32[0],
			-matrix_.r[2].m128_f32[1],
			-matrix_.r[2].m128_f32[2] };
	}

	inline Matrix Matrix::CreateTranslation(const Vector3& position) noexcept
	{
		return Matrix(DirectX::XMMatrixTranslationFromVector(position));
	}

	inline Matrix Matrix::CreateTranslation(const float x, const float y, const float z) noexcept
	{
		return Matrix(DirectX::XMMatrixTranslation(x, y, z));
	}

	inline Matrix Matrix::CreateScale(const Vector3& scales) noexcept
	{
		return Matrix(DirectX::XMMatrixScalingFromVector(scales));
	}

	inline Matrix Matrix::CreateScale(const float xs, const float ys, const float zs) noexcept
	{
		return Matrix(DirectX::XMMatrixScaling(xs, ys, zs));
	}

	inline Matrix Matrix::CreateScale(const float scale) noexcept
	{
		return Matrix(DirectX::XMMatrixScaling(scale, scale, scale));
	}

	inline Matrix Matrix::CreateRotationX(const float radians) noexcept
	{
		return Matrix(DirectX::XMMatrixRotationX(radians));
	}

	inline Matrix Matrix::CreateRotationY(const float radians) noexcept
	{
		return Matrix(DirectX::XMMatrixRotationY(radians));
	}

	inline Matrix Matrix::CreateRotationZ(const float radians) noexcept
	{
		return Matrix(DirectX::XMMatrixRotationZ(radians));
	}

	inline Matrix Matrix::CreateFromAxisAngle(const Vector3& axis, const float angle) noexcept
	{
		return Matrix(DirectX::XMMatrixRotationAxis(axis, angle));
	}

	inline Matrix Matrix::CreatePerspectiveFieldOfView(
		const float fov,
		const float aspectRatio,
		const float nearPlane,
		const float farPlane) noexcept
	{
		return Matrix(DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane));
	}

	inline Matrix Matrix::CreatePerspective(const float width, const float height, const float nearPlane, const float farPlane) noexcept
	{
		return Matrix(DirectX::XMMatrixPerspectiveLH(width, height, nearPlane, farPlane));
	}

	inline Matrix Matrix::CreatePerspectiveOffCenter(
		const float left,
		const float right,
		const float bottom,
		const float top,
		const float nearPlane,
		const float farPlane) noexcept
	{
		return Matrix(DirectX::XMMatrixPerspectiveOffCenterLH(left, right, bottom, top, nearPlane, farPlane));
	}

	inline Matrix Matrix::CreateOrthographic(
		const float width,
		const float height,
		const float zNearPlane,
		const float zFarPlane) noexcept
	{
		return Matrix(DirectX::XMMatrixOrthographicLH(width, height, zNearPlane, zFarPlane));
	}

	inline Matrix Matrix::CreateOrthographicOffCenter(
		const float left,
		const float right,
		const float bottom,
		const float top,
		const float zNearPlane,
		const float zFarPlane) noexcept
	{
		return Matrix(DirectX::XMMatrixOrthographicOffCenterLH(left, right, bottom, top, zNearPlane, zFarPlane));
	}

	inline Matrix Matrix::CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& up) noexcept
	{
		return Matrix(DirectX::XMMatrixLookAtRH(position, target, up));
	}

	inline Matrix Matrix::CreateWorld(const Vector3& position, const Vector3& forward, const Vector3& up) noexcept
	{
		Vector3 zaxis = Normalize(-forward);
		Vector3 xaxis = Normalize(Cross(Normalize(up), zaxis));
		Vector3 yaxis = Cross(zaxis, xaxis);

		return {
			xaxis[0], xaxis[1], xaxis[2], 0.0f,
			yaxis[0], yaxis[1], yaxis[2], 0.0f,
			zaxis[0], zaxis[1], zaxis[2], 0.0f,
			position[0], position[1], position[2], 1.0f };
	}

	inline Matrix Matrix::CreateFromYawPitchRoll(const float yaw, const float pitch, const float roll) noexcept
	{
		return Matrix(DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
	}

	inline void Matrix::Lerp(const Matrix& m1, const Matrix& m2, float t, Matrix& result) noexcept
	{
		
	}

	inline Matrix Matrix::Lerp(const Matrix& m1, const Matrix& m2, float t) noexcept
	{

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

}