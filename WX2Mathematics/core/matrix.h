/*********************************************************************
 * @file   matrix.h
 * @author Tomomi Murakami
 * @date   2021/12/05 13:32
 * @brief  XMMATRIXのラッパークラス
 ********************************************************************/
#pragma once
#include <DirectXMath.h>

namespace wx2
{
	class Matrix final
	{
	private:
		using XMMATRIX = DirectX::XMMATRIX;

	public:
		constexpr Matrix() noexcept;
		constexpr Matrix(
			const float m00, const float m01, const float m02, const float m03,
			const float m10, const float m11, const float m12, const float m13,
			const float m20, const float m21, const float m22, const float m23,
			const float m30, const float m31, const float m32, const float m33) noexcept;
		constexpr Matrix(
			const DirectX::XMVECTOR& r0, 
			const DirectX::XMVECTOR& r1,
			const DirectX::XMVECTOR& r2, 
			const DirectX::XMVECTOR& r3) noexcept;
		~Matrix() = default;

		Matrix(const Matrix&) = default;
		Matrix& operator=(const Matrix&) = default;
		Matrix(Matrix&&) = default;
		Matrix& operator=(Matrix&&) = default;

		explicit Matrix(const XMMATRIX& other) noexcept;
		Matrix& operator=(const XMMATRIX& rhs) noexcept;
		explicit Matrix(XMMATRIX&& other) noexcept;
		Matrix& operator=(XMMATRIX&& rhs) noexcept;

		static Matrix Identity() { return Matrix(DirectX::XMMatrixIdentity()); }

	private:
		XMMATRIX matrix_;
	};
}
