#pragma once 
#include "matrix.h"

namespace wx2
{
	constexpr Matrix::Matrix() noexcept
		: matrix_{
			{1.0f, 0.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}}
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
			{m30, m31, m32, m33}}
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

	inline Vector3 Matrix::operator[](const std::size_t index) const noexcept
	{
		WX2_ASSERT_MSG(index < 4, "“Y‚¦Žš‚Ì’l‚ª”ÍˆÍŠO‚Å‚·B");
		return matrix_.r[index];
	}

	inline constexpr Matrix Matrix::Identity() noexcept
	{
		return {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };
	}

	inline Vector3 Matrix::Right() const noexcept
	{
		return {
			matrix_.r[1].m128_f32[1],
			matrix_.r[1].m128_f32[2],
			matrix_.r[1].m128_f32[3], };
	}

	inline Vector3 Matrix::Left() const noexcept
	{
		return {
			-matrix_.r[1].m128_f32[1],
			-matrix_.r[1].m128_f32[2],
			-matrix_.r[1].m128_f32[3], };
	}

	inline Vector3 Matrix::Up() const noexcept
	{
		return {
			matrix_.r[2].m128_f32[1],
			matrix_.r[2].m128_f32[2],
			matrix_.r[2].m128_f32[3], };
	}

	inline Vector3 Matrix::Down() const noexcept
	{
		return {
			-matrix_.r[2].m128_f32[1],
			-matrix_.r[2].m128_f32[2],
			-matrix_.r[2].m128_f32[3], };
	}

	inline Vector3 Matrix::Backward() const noexcept
	{
		return {
			matrix_.r[3].m128_f32[1],
			matrix_.r[3].m128_f32[2],
			matrix_.r[3].m128_f32[3], };
	}

	inline Vector3 Matrix::Forward() const noexcept
	{
		return {
			-matrix_.r[3].m128_f32[1],
			-matrix_.r[3].m128_f32[2],
			-matrix_.r[3].m128_f32[3], };
	}
}
