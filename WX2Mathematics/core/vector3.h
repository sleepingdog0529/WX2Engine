#pragma once
#include <DirectXMath.h>

namespace wx2
{
	class Vector3 final
	{
	public:
		constexpr Vector3()
			: vector_{} {}
		constexpr Vector3(const float x, const float y, const float z)
			: vector_{ x, y, z } {}
		explicit constexpr Vector3(const float f)
			: vector_{ f, f, f } {}
		
		Vector3(const Vector3&) = default;
		Vector3& operator = (const Vector3&) = default;
		Vector3(Vector3&&) = default;
		Vector3& operator = (Vector3&&) = default;

		// à√ñŸÇÃå^ïœä∑Çãñâ¬
		constexpr operator DirectX::XMVECTOR() const noexcept { return vector_; }

		// î‰ärââéZéq
		bool operator == (const Vector3& rhs) const noexcept;
		bool operator != (const Vector3& rhs) const noexcept;

		// íPçÄââéZéq
		Vector3 operator+ () const noexcept { return *this; }
		Vector3 operator- () const noexcept;

		// ìÒçÄââéZéq
		Vector3 operator + (const Vector3& rhs) const noexcept;
		Vector3 operator - (const Vector3& rhs) const noexcept;
		Vector3 operator * (const float rhs) const noexcept;
		Vector3 operator / (const float rhs) const noexcept;

		// ë„ì¸ââéZéq
		Vector3& operator += (const Vector3& rhs) noexcept;
		Vector3& operator -= (const Vector3& rhs) noexcept;
		Vector3& operator *= (const float rhs) noexcept;
		Vector3& operator /= (const float rhs) noexcept;

		[[nodiscard]] float GetX() const noexcept { return DirectX::XMVectorGetX(vector_); }
		[[nodiscard]] float GetY() const noexcept { return DirectX::XMVectorGetY(vector_); }
		[[nodiscard]] float GetZ() const noexcept { return DirectX::XMVectorGetZ(vector_); }

		void SetX(const float x) noexcept { vector_ = DirectX::XMVectorSetX(vector_, x); }
		void SetY(const float y) noexcept { vector_ = DirectX::XMVectorSetY(vector_, y); }
		void SetZ(const float z) noexcept { vector_ = DirectX::XMVectorSetZ(vector_, z); }

		[[nodiscard]] float Length() const noexcept;
		[[nodiscard]] float LengthSquared() const noexcept;

	private:
		DirectX::XMVECTOR vector_;
	};
}

#include "vector3.inl"