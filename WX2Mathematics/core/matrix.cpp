#include "matrix.h"
#include <algorithm>

namespace wx2
{
	using namespace DirectX;

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
		const XMVECTOR& r0, 
		const XMVECTOR& r1, 
		const XMVECTOR& r2, 
		const XMVECTOR& r3) noexcept
		: matrix_(r0, r1, r2, r3)
	{
	}

	Matrix::Matrix(const XMMATRIX& other) noexcept
		: matrix_(other)
	{
	}

	Matrix& Matrix::operator=(const XMMATRIX& rhs) noexcept
	{
		matrix_ = rhs;
		return *this;
	}

	Matrix::Matrix(XMMATRIX&& other) noexcept
		: matrix_(std::move(other))
	{
	}
	
	Matrix& Matrix::operator=(XMMATRIX&& rhs) noexcept
	{
		matrix_ = std::move(rhs);
		return *this;
	}
}
