/*********************************************************************
 * @file   vector4.h
 * @author Tomomi Murakami
 * @date   2021/12/05 13:59
 * @brief  XMVECTORラッパークラス
 ********************************************************************/
#pragma once
#include <algorithm>
#include <DirectXMath.h>
#include <WX2Common.h>

namespace wx2
{
	class Vector4 final
	{
	public:
		constexpr Vector4() noexcept : vector_{} {}
		constexpr Vector4(const float x, const float y, const float z, const float w) noexcept
		: vector_{ x, y, z, w } {}
		explicit constexpr Vector4(const float f) noexcept
			: vector_{ f, f, f, f } {}
		~Vector4() = default;

		// コピー&ムーブ
		Vector4(const Vector4&) = default;
		Vector4& operator = (const Vector4&) = default;
		Vector4(Vector4&&) = default;
		Vector4& operator = (Vector4&&) = default;

		// XMVECTORからのコピー&ムーブ
		explicit Vector4(const DirectX::XMVECTOR& other) noexcept : vector_(other) {}
		Vector4& operator = (const DirectX::XMVECTOR& rhs) noexcept { vector_ = rhs; return *this; }
		explicit Vector4(DirectX::XMVECTOR&& other) noexcept : vector_(std::move(other)) {}
		Vector4& operator = (DirectX::XMVECTOR&& rhs) noexcept { vector_ = std::move(rhs); return *this; }

		// 暗黙の型変換を許可
		constexpr operator DirectX::XMVECTOR() const noexcept { return vector_; }
		operator DirectX::XMFLOAT4() const noexcept;

		// 比較演算子
		bool operator == (const Vector4& rhs) const noexcept;
		bool operator != (const Vector4& rhs) const noexcept;

		// 単項演算子
		Vector4 operator+ () const noexcept;
		Vector4 operator- () const noexcept;

		// 二項演算子
		Vector4 operator + (const Vector4& rhs) const noexcept;
		Vector4 operator - (const Vector4& rhs) const noexcept;
		Vector4 operator * (const float rhs) const noexcept;
		Vector4 operator / (const float rhs) const noexcept;

		// 代入演算子
		Vector4& operator += (const Vector4& rhs) noexcept;
		Vector4& operator -= (const Vector4& rhs) noexcept;
		Vector4& operator *= (const float rhs) noexcept;
		Vector4& operator /= (const float rhs) noexcept;

		// ゲッター
		[[nodiscard]] float GetX() const noexcept { return DirectX::XMVectorGetX(vector_); }
		[[nodiscard]] float GetY() const noexcept { return DirectX::XMVectorGetY(vector_); }
		[[nodiscard]] float GetZ() const noexcept { return DirectX::XMVectorGetZ(vector_); }
		[[nodiscard]] float GetW() const noexcept { return DirectX::XMVectorGetW(vector_); }

		// セッター
		void SetX(const float x) noexcept { vector_ = DirectX::XMVectorSetX(vector_, x); }
		void SetY(const float y) noexcept { vector_ = DirectX::XMVectorSetY(vector_, y); }
		void SetZ(const float z) noexcept { vector_ = DirectX::XMVectorSetZ(vector_, z); }
		void SetW(const float w) noexcept { vector_ = DirectX::XMVectorSetW(vector_, w); }

		void Normalized() noexcept;

		[[nodiscard]] float Length() const noexcept;
		[[nodiscard]] float LengthSquared() const noexcept;

		static Vector4 Normalize(const Vector4& v) noexcept;
		static float Dot(const Vector4& v1, const Vector4& v2) noexcept;
		static float Distance(const Vector4& v1, const Vector4& v2) noexcept;
		static float DistanceSquared(const Vector4& v1, const Vector4& v2) noexcept;
		static Vector4 Cross(const Vector4& v1, const Vector4& v2, const Vector4& v3) noexcept;

		static constexpr Vector4 Zero()  noexcept { return { 0.0f, 0.0f, 0.0f, 0.0f }; }
		static constexpr Vector4 One()   noexcept { return { 1.0f, 1.0f, 1.0f, 1.0f }; }
		static constexpr Vector4 UnitX() noexcept { return { 1.0f, 0.0f, 0.0f, 0.0f }; }
		static constexpr Vector4 UnitY() noexcept { return { 0.0f, 1.0f, 0.0f, 0.0f }; }
		static constexpr Vector4 UnitZ() noexcept { return { 0.0f, 0.0f, 1.0f, 0.0f }; }
		static constexpr Vector4 UnitW() noexcept { return { 0.0f, 0.0f, 0.0f, 1.0f }; }

	private:
		DirectX::XMVECTOR vector_{};
	};

#include "vector4.inl"
}