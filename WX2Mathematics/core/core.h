/*********************************************************************
 * @file   core.h
 * @author Tomomi Murakami
 * @date   2021/10/13 0:59
 * @brief  スカラー、ベクトル、マトリックス演算
 * @brief  参考: https://github.com/microsoft/DirectXTK
 ********************************************************************/
#pragma once
#include <DirectXMath.h>

namespace wx2
{
	static constexpr float PI = DirectX::XM_PI;
	static constexpr float TAU = DirectX::XM_2PI;
	static constexpr float PIDIV2 = DirectX::XM_PIDIV2;
	static constexpr float PIDIV4 = DirectX::XM_PIDIV4;

	/**
	 * @brief  正の値なら-1, 負の値なら-1, 0なら0を返す
	 * @param  val 判定する値
	 * @return 符号の値
	 */
	template <typename T>
	constexpr T Sign(T val) noexcept
	{
		return (val > T(0)) - (val < T(0));
	}

	/**
	 * @brief  ある範囲内にある値を別の範囲に当てはめ変換する
	 * @param  val 変換する値
	 * @param  inMin 最小入力範囲
	 * @param  inMax 最大入力範囲
	 * @param  outMin 最小出力範囲
	 * @param  outMax 最大出力範囲
	 * @return 変換した値
	 */
	template <typename T>
	constexpr T Remap(T val, T inMin, T inMax, T outMin, T outMax) noexcept
	{
		return (val - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
	}

	class Vector3 final
	{
	public:
		constexpr Vector3() noexcept
			: vector_{} {}
		explicit constexpr Vector3(const DirectX::XMVECTOR& xmv) noexcept
			: vector_(xmv) {}
		constexpr Vector3(const float x, const float y, const float z) noexcept
			: vector_{ x, y, z } {}
		explicit constexpr Vector3(const float f) noexcept
			: vector_{ f, f, f } {}
		~Vector3() = default;

		Vector3(const Vector3&) = default;
		Vector3& operator = (const Vector3&) = default;
		Vector3(Vector3&&) = default;
		Vector3& operator = (Vector3&&) = default;

		// 比較演算子
		bool operator == (const Vector3& rhs) const noexcept;
		bool operator != (const Vector3& rhs) const noexcept;

		// 単項演算子
		Vector3 operator+ () const noexcept { return *this; }
		Vector3 operator- () const noexcept;

		// 二項演算子
		Vector3 operator + (const Vector3& rhs) const noexcept;
		Vector3 operator - (const Vector3& rhs) const noexcept;
		Vector3 operator * (float rhs) const noexcept;
		Vector3 operator / (float rhs) const noexcept;

		// 代入演算子
		Vector3& operator += (const Vector3& rhs) noexcept;
		Vector3& operator -= (const Vector3& rhs) noexcept;
		Vector3& operator *= (float rhs) noexcept;
		Vector3& operator /= (float rhs) noexcept;

		const float& operator [] (std::size_t index) const noexcept;
		float& operator [] (std::size_t index) noexcept;

		operator DirectX::XMVECTOR() const noexcept;

		[[nodiscard]] float GetX() const noexcept { return DirectX::XMVectorGetX(vector_); }
		[[nodiscard]] float GetY() const noexcept { return DirectX::XMVectorGetY(vector_); }
		[[nodiscard]] float GetZ() const noexcept { return DirectX::XMVectorGetZ(vector_); }

		void SetX(const float x) noexcept { vector_ = DirectX::XMVectorSetX(vector_, x); }
		void SetY(const float y) noexcept { vector_ = DirectX::XMVectorSetY(vector_, y); }
		void SetZ(const float z) noexcept { vector_ = DirectX::XMVectorSetZ(vector_, z); }

		void Normalized() noexcept;

		[[nodiscard]] float Length() const noexcept;
		[[nodiscard]] float LengthSquared() const noexcept;

		static constexpr Vector3 Zero()		noexcept { return{  0.0f,  0.0f,  0.0f }; }
		static constexpr Vector3 One()		noexcept { return{  1.0f,  1.0f,  1.0f }; }
		static constexpr Vector3 Right()	noexcept { return{  1.0f,  0.0f,  0.0f }; }
		static constexpr Vector3 Left()		noexcept { return{ -1.0f,  0.0f,  0.0f }; }
		static constexpr Vector3 Up()		noexcept { return{  0.0f,  1.0f,  0.0f }; }
		static constexpr Vector3 Down()		noexcept { return{  0.0f, -1.0f,  0.0f }; }
		static constexpr Vector3 Forward()	noexcept { return{  0.0f,  0.0f,  1.0f }; }
		static constexpr Vector3 Backward()	noexcept { return{  0.0f,  0.0f, -1.0f }; }

	private:
		DirectX::XMVECTOR vector_;
	};

	static Vector3 Normalize(const Vector3& v) noexcept;
	static float Dot(const Vector3& v1, const Vector3& v2) noexcept;
	static Vector3 Cross(const Vector3& v1, const Vector3& v2) noexcept;

	class Vector4 final
	{
	public:
		constexpr Vector4() noexcept
			: vector_{} {}
		explicit constexpr Vector4(const DirectX::XMVECTOR& xmv) noexcept
			: vector_(xmv) {}
		constexpr Vector4(const float x, const float y, const float z, const float w) noexcept
			: vector_{ x, y, z, w } {}
		explicit constexpr Vector4(const float f) noexcept
			: vector_{ f, f, f, f } {}
		~Vector4() = default;

		Vector4(const Vector4&) = default;
		Vector4& operator = (const Vector4&) = default;
		Vector4(Vector4&&) = default;
		Vector4& operator = (Vector4&&) = default;

		// 比較演算子
		bool operator == (const Vector4& rhs) const noexcept;
		bool operator != (const Vector4& rhs) const noexcept;

		// 単項演算子
		Vector4 operator+ () const noexcept { return *this; }
		Vector4 operator- () const noexcept;

		// 二項演算子
		Vector4 operator + (const Vector4& rhs) const noexcept;
		Vector4 operator - (const Vector4& rhs) const noexcept;
		Vector4 operator * (float rhs) const noexcept;
		Vector4 operator / (float rhs) const noexcept;

		// 代入演算子
		Vector4& operator += (const Vector4& rhs) noexcept;
		Vector4& operator -= (const Vector4& rhs) noexcept;
		Vector4& operator *= (float rhs) noexcept;
		Vector4& operator /= (float rhs) noexcept;

		const float& operator [] (std::size_t index) const noexcept;
		float& operator [] (std::size_t index) noexcept;

		operator DirectX::XMVECTOR() const noexcept;

		[[nodiscard]] float GetX() const noexcept { return DirectX::XMVectorGetX(vector_); }
		[[nodiscard]] float GetY() const noexcept { return DirectX::XMVectorGetY(vector_); }
		[[nodiscard]] float GetZ() const noexcept { return DirectX::XMVectorGetZ(vector_); }
		[[nodiscard]] float GetW() const noexcept { return DirectX::XMVectorGetW(vector_); }

		void SetX(const float x) noexcept { vector_ = DirectX::XMVectorSetX(vector_, x); }
		void SetY(const float y) noexcept { vector_ = DirectX::XMVectorSetY(vector_, y); }
		void SetZ(const float z) noexcept { vector_ = DirectX::XMVectorSetZ(vector_, z); }
		void SetW(const float w) noexcept { vector_ = DirectX::XMVectorSetW(vector_, w); }

		void Normalized() noexcept;

		[[nodiscard]] float Length() const noexcept;
		[[nodiscard]] float LengthSquared() const noexcept;

		static constexpr Vector4 Zero()		noexcept { return{ 0.0f, 0.0f, 0.0f, 0.0f }; }
		static constexpr Vector4 One()		noexcept { return{ 1.0f, 1.0f, 1.0f, 1.0f }; }
		static constexpr Vector4 UnitX()	noexcept { return{ 1.0f, 0.0f, 0.0f, 0.0f }; }
		static constexpr Vector4 UnitY()	noexcept { return{ 0.0f, 1.0f, 0.0f, 0.0f }; }
		static constexpr Vector4 UnitZ()	noexcept { return{ 0.0f, 0.0f, 1.0f, 0.0f }; }
		static constexpr Vector4 UnitW()	noexcept { return{ 0.0f, 0.0f, 0.0f, 1.0f }; }

	private:
		DirectX::XMVECTOR vector_;
	};

	class Matrix final
	{
	public:
		constexpr Matrix() noexcept;
		explicit constexpr Matrix(const DirectX::XMMATRIX& xmm) noexcept
			: matrix_(xmm) {}
		constexpr Matrix(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33) noexcept;
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

		bool operator == (const Matrix& rhs) const noexcept;
		bool operator != (const Matrix& rhs) const noexcept;

		const Vector4& operator [] (const std::size_t index) const noexcept;
		Vector4& operator [] (const std::size_t index) noexcept;

		[[nodiscard]] constexpr Vector3 Right()    const noexcept;
		[[nodiscard]] constexpr Vector3 Left()     const noexcept;
		[[nodiscard]] constexpr Vector3 Up()       const noexcept;
		[[nodiscard]] constexpr Vector3 Down()     const noexcept;
		[[nodiscard]] constexpr Vector3 Forward()  const noexcept;
		[[nodiscard]] constexpr Vector3 Backward() const noexcept;

		static Matrix CreateTranslation(const Vector3& position) noexcept;
		static Matrix CreateTranslation(const float x, const float y, const float z) noexcept;

		static Matrix CreateScale(const Vector3& scales) noexcept;
		static Matrix CreateScale(float xs, float ys, float zs) noexcept;
		static Matrix CreateScale(float scale) noexcept;

		static Matrix CreateRotationX(const float radians) noexcept;
		static Matrix CreateRotationY(const float radians) noexcept;
		static Matrix CreateRotationZ(const float radians) noexcept;

		static Matrix CreateFromAxisAngle(const Vector3& axis, float angle) noexcept;

		static Matrix CreatePerspectiveFieldOfView(float fov, float aspectRatio, float nearPlane, float farPlane) noexcept;
		static Matrix CreatePerspective(float width, float height, float nearPlane, float farPlane) noexcept;
		static Matrix CreatePerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane) noexcept;
		static Matrix CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane) noexcept;
		static Matrix CreateOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane) noexcept;

		static Matrix CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& up) noexcept;
		static Matrix CreateWorld(const Vector3& position, const Vector3& forward, const Vector3& up) noexcept;

		static Matrix CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept;

		static void Lerp(const Matrix& m1, const Matrix& m2, float t, Matrix& result) noexcept;
		static Matrix Lerp(const Matrix& m1, const Matrix& m2, float t) noexcept;

		static constexpr Matrix Identity() noexcept;

	private:
		DirectX::XMMATRIX matrix_;
	};
}

#include "core.inl"