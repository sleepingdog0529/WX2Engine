/*********************************************************************
 * @file   core.h
 * @author Tomomi Murakami
 * @date   2021/10/13 0:59
 * @brief  �X�J���[�A�x�N�g���A�}�g���b�N�X���Z
 * @brief  �Q�l: https://github.com/microsoft/DirectXTK
 * @brief  �x�N�g���̃T�C�Y�͑S��4�o�C�g�Ȃ̂Œ���
********************************************************************/
#pragma once
#include <DirectXMath.h>
#include <WX2Common.h>

namespace wx2
{
	class Vector2;
	class Vector3;
	class Vector4;
	class Matrix;
	class Quaternion;

	//! @brief ��
	static constexpr float PI = DirectX::XM_PI;
	//! @brief �� (�� * 2)
	static constexpr float TAU = DirectX::XM_2PI;
	//! @brief �� / 2
	static constexpr float PIDIV2 = DirectX::XM_PIDIV2;
	//! @brief �� / 4
	static constexpr float PIDIV4 = DirectX::XM_PIDIV4;

	/**
	 * @brief  ���̒l�Ȃ�-1, ���̒l�Ȃ�-1, 0�Ȃ�0��Ԃ�
	 * @param  v ���肷��l
	 * @return �����̒l
	 */
	template <typename T>
	constexpr T Sign(T v) noexcept
	{
		return (v > T(0)) - (v < T(0));
	}

	/**
	 * @brief  ����͈͓��ɂ���l��ʂ͈̔͂ɓ��Ă͂ߕϊ�����
	 * @param  v �ϊ�����l
	 * @param  inMin �ŏ����͔͈�
	 * @param  inMax �ő���͔͈�
	 * @param  outMin �ŏ��o�͔͈�
	 * @param  outMax �ő�o�͔͈�
	 * @return �ϊ������l
	 */
	template <typename T>
	constexpr T Remap(T v, T inMin, T inMax, T outMin, T outMax) noexcept
	{
		return (v - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
	}

	/**
	 * @brief  �񎟌��x�N�g���N���X(4Byte)
	 */
	class Vector2 final
	{
	public:
		/**
		 * @brief  2D�x�N�g���̑S�Ă̐�����0�ŏ���������
		 */
		constexpr Vector2() noexcept
			: vector_{} {}
		/**
		 * @brief  X,Y���W����2D�x�N�g�����\�z����
		 * @param  x X���W�̒l
		 * @param  y Y���W�̒l
		 */
		constexpr Vector2(const float x, const float y) noexcept
			: vector_{ x, y } {}
		/**
		 * @brief  �z�񂩂�2D�x�N�g�����\�z����
		 * @param  array �v�f�̒l���Z�b�g���ꂽ�z��|�C���^
		 */
		explicit constexpr Vector2(_In_reads_(2) const float* array) noexcept
			: vector_{ array[0], array[1]} {}
		/**
		 * @brief  �S�Ă̗v�f�������l��2D�x�N�g�����\�z����
		 * @param  f �v�f�̒l
		 */
		explicit constexpr Vector2(const float f) noexcept
			: vector_{ f, f } {}
		/**
		 * @brief  XMVECTOR�^����2D�x�N�g�����\�z����
		 * @param  xmv XMVECTOR
		 */
		explicit constexpr Vector2(const DirectX::XMVECTOR& xmv) noexcept
			: vector_{ xmv.m128_f32[0], xmv.m128_f32[2] } {}
		/**
		 * @brief  XMFLOAT2�^����2D�x�N�g�����\�z����
		 * @param  xmf XMFLOAT
		 */
		explicit Vector2(const DirectX::XMFLOAT2& xmf) noexcept
			: vector_(DirectX::XMLoadFloat2(&xmf)) {}

		~Vector2() = default;

		Vector2(const Vector2&) = default;
		Vector2& operator = (const Vector2&) = default;
		Vector2(Vector2&&) = default;
		Vector2& operator = (Vector2&&) = default;

		// ��r���Z�q
		bool operator ==(const Vector2& rhs) const noexcept;
		bool operator != (const Vector2& rhs) const noexcept;

		// �P�����Z�q
		constexpr Vector2 operator + () const noexcept { return *this; }
		Vector2 operator - () const noexcept;

		// �񍀉��Z�q
		Vector2 operator + (const Vector2& rhs) const noexcept;
		Vector2 operator - (const Vector2& rhs) const noexcept;
		Vector2 operator * (float rhs) const noexcept;
		Vector2 operator / (float rhs) const noexcept;

		// ������Z�q
		Vector2& operator += (const Vector2& rhs) noexcept;
		Vector2& operator -= (const Vector2& rhs) noexcept;
		Vector2& operator *= (float rhs) noexcept;
		Vector2& operator /= (float rhs) noexcept;

		// �Y�����Z�q
		const float& operator [] (std::size_t index) const noexcept;
		float& operator [] (std::size_t index) noexcept;

		// �L���X�g���Z�q
		operator DirectX::XMVECTOR() const noexcept;
		operator DirectX::XMFLOAT2() const noexcept;

		/**
		 * @brief  2D�x�N�g����X�������擾����
		 * @return X�����̒l
		 */
		[[nodiscard]] float X() const noexcept { return DirectX::XMVectorGetX(vector_); }
		/**
		 * @brief  2D�x�N�g����Y�������擾����
		 * @return Y�����̒l
		 */
		[[nodiscard]] float Y() const noexcept { return DirectX::XMVectorGetY(vector_); }

		/**
		 * @brief  2D�x�N�g����X������ݒ肷��
		 * @param  x ����������X�����̒l
		 */
		void X(const float x) noexcept { vector_ = DirectX::XMVectorSetX(vector_, x); }
		/**
		 * @brief  2D�x�N�g����Y������ݒ肷��
		 * @param  y ����������Y�����̒l
		 */
		void Y(const float y) noexcept { vector_ = DirectX::XMVectorSetY(vector_, y); }

		/**
		 * @brief  2D�x�N�g���𐳋K������
		 */
		void Normalized() noexcept;

		/**
		 * @brief  2D�x�N�g���̒��������߂�
		 * @return 2D�x�N�g���̒���
		 */
		[[nodiscard]] float Length() const noexcept;
		/**
		 * @brief  2D�x�N�g���̒����̕��������߂�
		 * @return 2D�x�N�g���̒����̕���
		 */
		[[nodiscard]] float LengthSquared() const noexcept;
		
		/**
		 * @brief  2D�x�N�g���̃��W�A���p�x�����߂�
		 * @return 2D�x�N�g���̃��W�A���p�x
		 */
		[[nodiscard]] float Angle() const noexcept;

		/**
		 * @param   v �e�X�g����2D�x�N�g��
		 * @param   bounds ���E�����߂�2D�x�N�g��
		 * @return  2D�x�N�g���̐������ݒ肵���͈͓��ɂ��邩
		 * @note https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvector2inbounds
		 */
		static bool InBounds(const Vector2& v, const Vector2& bounds) noexcept;

		/**
		 * @brief  2D�x�N�g���Ԃ̋��������߂�
		 * @param  v1 2D�x�N�g��
		 * @param  v2 2D�x�N�g��
		 * @return �x�N�g���Ԃ̋���
		 */
		static float Distance(const Vector2& v1, const Vector2& v2) noexcept;

		/**
		 * @brief  2D�x�N�g���Ԃ̋����̕��������߂�
		 * @param  v1 2D�x�N�g��
		 * @param  v2 2D�x�N�g��
		 * @return �x�N�g���Ԃ̋����̕���
		 */
		static float DistanceSquared(const Vector2& v1, const Vector2& v2) noexcept;
		
		/**
		 * @brief  2D�x�N�g���̓��ς����߂�
		 * @param  v1 2D�x�N�g��
		 * @param  v2 2D�x�N�g��
		 * @return 2D�x�N�g���̓���
		 */
		static float Dot(const Vector2& v1, const Vector2& v2) noexcept;
		/**
		 * @brief  2D�x�N�g���̊O�ς����߂�
		 * @param  v1 2D�x�N�g��
		 * @param  v2 2D�x�N�g��
		 * @return 2D�x�N�g���̊O��
		 */
		static Vector2 Cross(const Vector2& v1, const Vector2& v2) noexcept;
		/**
		 * @brief  2D�x�N�g���𐳋K������
		 * @param  v 2D�x�N�g��
		 * @return ���K�����ꂽ2D�x�N�g��
		 */
		static Vector2 Normalize(const Vector2& v) noexcept;
		/**
		 * @brief  2D�x�N�g���̐������w�肵���ŏ��l�ƍő�l�͈̔͂ɃN�����v����
		 * @param  v �N�����v����鐬��������2D�x�N�g��
		 * @param  vmin �ŏ��͈͂̐���������2D�x�N�g��
		 * @param  vmax �ő�͈͂̐���������2D�x�N�g��
		 * @return �N�����v���ꂽ2D�x�N�g��
		 */
		static Vector2 Clamp(const Vector2& v, const Vector2& vmin, const Vector2& vmax) noexcept;
		/**
		 * @brief  2��2D�x�N�g���̐������Ƃ̔�r���s���A�ŏ��̐���������2D�x�N�g����Ԃ�
		 * @param  v1 2D�x�N�g��
		 * @param  v2 2D�x�N�g��
		 * @return �ŏ��̐�����������2D�x�N�g��
		 */
		static Vector2 Min(const Vector2& v1, const Vector2& v2) noexcept;
		/**
		 * @brief  2��2D�x�N�g���̐������Ƃ̔�r���s���A�ő�̐���������2D�x�N�g����Ԃ�
		 * @param  v1 2D�x�N�g��
		 * @param  v2 2D�x�N�g��
		 * @return �ő�̐�����������2D�x�N�g��
		 */
		static Vector2 Max(const Vector2& v1, const Vector2& v2) noexcept;
		/**
		 * @brief  2D�x�N�g���Ԃ̐��`��Ԃ��s��
		 * @param  v1 2D�n�_�x�N�g��
		 * @param  v2 2D�I�_�x�N�g��
		 * @param  t ��Ԑ���W��
		 * @return �⊮���ʂ�\��2D�x�N�g��
		 */
		static Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t) noexcept;
		/**
		 * @brief  2D�x�N�g���Ԃ��G���~�[�g��������p���Đ��`��Ԃ��s��
		 * @param  v1 2D�n�_�x�N�g��
		 * @param  v2 2D�I�_�x�N�g��
		 * @param  t ��Ԑ���W��
		 * @return �⊮���ʂ�\��2D�x�N�g��
		 */
		static Vector2 SmoothStep(const Vector2& v1, const Vector2& v2, float t) noexcept;
		/**
		 * @brief  3��2D�ʒu�x�N�g����p����2D�΍��d�S���W�x�N�g����Ԃ�
		 * @param  v1 1�Ԗڂ̏d�S�ʒu��\��2D�x�N�g��
		 * @param  v2 2�Ԗڂ̏d�S�ʒu��\��2D�x�N�g��
		 * @param  v3 3�Ԗڂ̏d�S�ʒu��\��2D�x�N�g��
		 * @param  f �d�ݕt���W��
		 * @param  g �d�ݕt���W��
		 * @return 2D�΍��d�S���W�x�N�g��
		 * @note   https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvectorbarycentric
		 */
		static Vector2 BaryCentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, float f, float g) noexcept;
		/**
		 * @brief  �w�肳�ꂽ2D�ʒu�x�N�g����p���ăJ�b�g���������X�v���C���Ȑ���Ԃ��s��
		 * @param  v1 1�ڂ̐���_��\��2D�x�N�g��
		 * @param  v2 2�ڂ̐���_��\��2D�x�N�g��
		 * @param  v3 3�ڂ̐���_��\��2D�x�N�g��
		 * @param  v4 4�ڂ̐���_��\��2D�x�N�g��
		 * @param  t ��Ԑ���W��
		 * @return �⊮���ʂ�\��2D�x�N�g��
		 * @note   https://docs.microsoft.com/ja-jp/windows/win32/api/directxmath/nf-directxmath-xmquaternionbarycentric
		 */
		static Vector2 CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, float t) noexcept;
		/**
		 * @brief  �w�肳�ꂽ2D�ʒu�x�N�g����p���ăG���~�[�g�Ȑ���Ԃ��s��
		 * @param  v1 1�ڂ̐���_�̈ʒu��\��2D�x�N�g��
		 * @param  t1 1�ڂ̐���_�̑��x��\��2D�x�N�g��
		 * @param  v2 2�ڂ̐���_�̈ʒu��\��2D�x�N�g��
		 * @param  t2 2�ڂ̐���_�̑��x��\��2D�x�N�g��
		 * @param  t ��Ԑ���W��
		 * @return �⊮���ʂ�\��2D�x�N�g��
		 * @note   https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvectorcatmullrom
		 */
		static Vector2 Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2, float t) noexcept;
		/**
		 * @brief  2D���˃x�N�g����2D�@���x�N�g���Ŕ��˂�����2D�x�N�g�������߂�
		 * @param  ivec 2D���˃x�N�g��
		 * @param  nvec 2D�@���x�N�g��
		 * @return2D ���˃x�N�g��
		 * @note   https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvectorhermite
		 */
		static Vector2 Reflect(const Vector2& ivec, const Vector2& nvec) noexcept;
		/**
		 * @brief  2D���˃x�N�g����2D�@���x�N�g���ŋ��܂�����2D�x�N�g�������߂�
		 * @param  ivec 2D���˃x�N�g��
		 * @param  nvec 2D�@���x�N�g��
		 * @param  refractionIndex ���ܗ�
		 * @return 2D���܃x�N�g��
		 */
		static Vector2 Refract(const Vector2& ivec, const Vector2& nvec, float refractionIndex) noexcept;
		/**
		 * @brief  �N�H�[�^�j�I�����g�p����2D�x�N�g������]����
		 * @param  v ��]����2D�x�N�g��
		 * @param  q 2D�x�N�g���ɉ�]��K������N�H�[�^�j�I��
		 * @return ��]����2D�x�N�g��
		 */
		static Vector2 Transform(const Vector2& v, const Quaternion& q) noexcept;
		/**
		 * @brief  �ϊ��s����g�p����2D�x�N�g����ϊ�����
		 * @param  v �ϊ�����2D�x�N�g��
		 * @param  m �ϊ��s��
		 * @return �ϊ����ꂽ2D�x�N�g��
		 */
		static Vector2 Transform(const Vector2& v, const Matrix& m) noexcept;
		/**
		 * @brief  �ϊ��s����g�p����2D�@���x�N�g����ϊ�����
		 * @param  v �ϊ�����2D�@���x�N�g��
		 * @param  m �ϊ��s��
		 * @return �ϊ����ꂽ2D�@���x�N�g��
		 */
		static Vector2 TransformNormal(const Vector2& v, const Matrix& m) noexcept;
		/**
		 * @brief  �p�x����2D�P�ʃx�N�g�����쐬����
		 * @param  angle ���W�A���p�x
		 * @return 2D�P�ʃx�N�g��
		 */
		static Vector2 FromAngle(float angle) noexcept;

		/**
		 * @brief  �S�Ă̐�����0�ł���2D�x�N�g����Ԃ�
		 * @return �S�Ă̐�����0��2D�x�N�g��
		 */
		static constexpr Vector2 Zero() noexcept { return{  0.0f,  0.0f }; }
		/**
		 * @brief  �S�Ă̐�����1�ł���2D�x�N�g����Ԃ�
		 * @return �S�Ă̐�����1��2D�x�N�g��
		 */
		static constexpr Vector2 One() noexcept { return{  1.0f,  1.0f }; }
		/**
		 * @brief  �E����(X+)��2D�P�ʃx�N�g����Ԃ�
		 * @return �E������2D�P�ʃx�N�g��
		 */
		static constexpr Vector2 Right() noexcept { return{  1.0f,  0.0f }; }
		/**
		 * @brief  ������(X-)��2D�P�ʃx�N�g����Ԃ�
		 * @return ��������2D�P�ʃx�N�g��
		 */
		static constexpr Vector2 Left()	noexcept { return{ -1.0f,  0.0f }; }
		/**
		 * @brief  �����(Y+)��2D�P�ʃx�N�g����Ԃ�
		 * @return �������2D�P�ʃx�N�g��
		 */
		static constexpr Vector2 Up() noexcept { return{  0.0f,  1.0f }; }
		/**
		 * @brief  ������(Y-)��2D�P�ʃx�N�g����Ԃ�
		 * @return ��������2D�P�ʃx�N�g��
		 */
		static constexpr Vector2 Down() noexcept { return{  0.0f, -1.0f }; }

	private:
		DirectX::XMVECTOR vector_;
	};

	class Vector3 final
	{
	public:
		constexpr Vector3() noexcept
			: vector_{} {}
		constexpr Vector3(const float x, const float y, const float z) noexcept
			: vector_{ x, y, z } {}
		explicit constexpr Vector3(_In_reads_(3) const float* array) noexcept
			: vector_{ array[0], array[1], array[2] } {}
		explicit constexpr Vector3(const float f) noexcept
			: vector_{ f, f, f } {}
		explicit constexpr Vector3(const DirectX::XMVECTOR& xmv) noexcept
			: vector_{ xmv.m128_f32[0], xmv.m128_f32[1], xmv.m128_f32[2] } {}
		~Vector3() = default;

		Vector3(const Vector3&) = default;
		Vector3& operator = (const Vector3&) = default;
		Vector3(Vector3&&) = default;
		Vector3& operator = (Vector3&&) = default;

		// ��r���Z�q
		bool operator == (const Vector3& rhs) const noexcept;
		bool operator != (const Vector3& rhs) const noexcept;

		// �P�����Z�q
		constexpr Vector3 operator + () const noexcept { return *this; }
		Vector3 operator - () const noexcept;

		// �񍀉��Z�q
		Vector3 operator + (const Vector3& rhs) const noexcept;
		Vector3 operator - (const Vector3& rhs) const noexcept;
		Vector3 operator * (float rhs) const noexcept;
		Vector3 operator / (float rhs) const noexcept;

		// ������Z�q
		Vector3& operator += (const Vector3& rhs) noexcept;
		Vector3& operator -= (const Vector3& rhs) noexcept;
		Vector3& operator *= (float rhs) noexcept;
		Vector3& operator /= (float rhs) noexcept;

		const float& operator [] (std::size_t index) const noexcept;
		float& operator [] (std::size_t index) noexcept;

		operator DirectX::XMVECTOR() const noexcept;

		[[nodiscard]] float X() const noexcept { return DirectX::XMVectorGetX(vector_); }
		[[nodiscard]] float Y() const noexcept { return DirectX::XMVectorGetY(vector_); }
		[[nodiscard]] float Z() const noexcept { return DirectX::XMVectorGetZ(vector_); }

		void X(const float x) noexcept { vector_ = DirectX::XMVectorSetX(vector_, x); }
		void Y(const float y) noexcept { vector_ = DirectX::XMVectorSetY(vector_, y); }
		void Z(const float z) noexcept { vector_ = DirectX::XMVectorSetZ(vector_, z); }

		void Normalized() noexcept;

		[[nodiscard]] float Length() const noexcept;
		[[nodiscard]] float LengthSquared() const noexcept;

		static bool InBounds(const Vector3& v, const Vector3& bounds) noexcept;
		static float Distance(const Vector3& v1, const Vector3& v2) noexcept;
		static float DistanceSquared(const Vector3& v1, const Vector3& v2) noexcept;
		static float Dot(const Vector3& v1, const Vector3& v2) noexcept;
		static Vector3 Cross(const Vector3& v1, const Vector3& v2) noexcept;
		static Vector3 Normalize(const Vector3& v) noexcept;
		static Vector3 Clamp(const Vector3& v, const Vector3& vmin, const Vector3& vmax) noexcept;
		static Vector3 Min(const Vector3& v1, const Vector3& v2) noexcept;
		static Vector3 Max(const Vector3& v1, const Vector3& v2) noexcept;
		static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) noexcept;
		static Vector3 SmoothStep(const Vector3& v1, const Vector3& v2, float t) noexcept;
		static Vector3 BaryCentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g) noexcept;
		static Vector3 CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t) noexcept;
		static Vector3 Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t) noexcept;
		static Vector3 Reflect(const Vector3& ivec, const Vector3& nvec) noexcept;
		static Vector3 Refract(const Vector3& ivec, const Vector3& nvec, float refractionIndex) noexcept;
		static Vector3 Transform(const Vector3& v, const Quaternion& q) noexcept;
		static Vector3 Transform(const Vector3& v, const Matrix& m) noexcept;
		static Vector3 TransformNormal(const Vector3& v, const Matrix& m) noexcept;

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

	class Vector4 final
	{
	public:
		constexpr Vector4() noexcept
			: vector_{} {}
		constexpr Vector4(const float x, const float y, const float z, const float w) noexcept
			: vector_{ x, y, z, w } {}
		explicit constexpr Vector4(_In_reads_(4) const float* array) noexcept
			: vector_{ array[0], array[1], array[2], array[3] } {}
		explicit constexpr Vector4(const float f) noexcept
			: vector_{ f, f, f, f } {}
		explicit constexpr Vector4(const DirectX::XMVECTOR& xmv) noexcept
			: vector_(xmv) {}
		~Vector4() = default;

		Vector4(const Vector4&) = default;
		Vector4& operator = (const Vector4&) = default;
		Vector4(Vector4&&) = default;
		Vector4& operator = (Vector4&&) = default;

		// ��r���Z�q
		bool operator == (const Vector4& rhs) const noexcept;
		bool operator != (const Vector4& rhs) const noexcept;

		// �P�����Z�q
		constexpr Vector4 operator + () const noexcept { return *this; }
		Vector4 operator - () const noexcept;

		// �񍀉��Z�q
		Vector4 operator + (const Vector4& rhs) const noexcept;
		Vector4 operator - (const Vector4& rhs) const noexcept;
		Vector4 operator * (float rhs) const noexcept;
		Vector4 operator / (float rhs) const noexcept;

		// ������Z�q
		Vector4& operator += (const Vector4& rhs) noexcept;
		Vector4& operator -= (const Vector4& rhs) noexcept;
		Vector4& operator *= (float rhs) noexcept;
		Vector4& operator /= (float rhs) noexcept;

		const float& operator [] (std::size_t index) const noexcept;
		float& operator [] (std::size_t index) noexcept;

		operator DirectX::XMVECTOR() const noexcept;

		[[nodiscard]] float X() const noexcept { return DirectX::XMVectorGetX(vector_); }
		[[nodiscard]] float Y() const noexcept { return DirectX::XMVectorGetY(vector_); }
		[[nodiscard]] float Z() const noexcept { return DirectX::XMVectorGetZ(vector_); }
		[[nodiscard]] float W() const noexcept { return DirectX::XMVectorGetW(vector_); }

		void X(const float x) noexcept { vector_ = DirectX::XMVectorSetX(vector_, x); }
		void Y(const float y) noexcept { vector_ = DirectX::XMVectorSetY(vector_, y); }
		void Z(const float z) noexcept { vector_ = DirectX::XMVectorSetZ(vector_, z); }
		void W(const float w) noexcept { vector_ = DirectX::XMVectorSetW(vector_, w); }

		void Normalized() noexcept;

		[[nodiscard]] float Length() const noexcept;
		[[nodiscard]] float LengthSquared() const noexcept;

		static bool InBounds(const Vector4& v, const Vector4& bounds) noexcept;
		static float Distance(const Vector4& v1, const Vector4& v2) noexcept;
		static float DistanceSquared(const Vector4& v1, const Vector4& v2) noexcept;
		static float Dot(const Vector4& v1, const Vector4& v2) noexcept;
		static Vector4 Cross(const Vector4& v1, const Vector4& v2, const Vector4& v3) noexcept;
		static Vector4 Normalize(const Vector4& v) noexcept;
		static Vector4 Clamp(const Vector4& v, const Vector4& vmin, const Vector4& vmax) noexcept;
		static Vector4 Min(const Vector4& v1, const Vector4& v2) noexcept;
		static Vector4 Max(const Vector4& v1, const Vector4& v2) noexcept;
		static Vector4 Lerp(const Vector4& v1, const Vector4& v2, float t) noexcept;
		static Vector4 SmoothStep(const Vector4& v1, const Vector4& v2, float t) noexcept;
		static Vector4 BaryCentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g) noexcept;
		static Vector4 CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t) noexcept;
		static Vector4 Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t) noexcept;
		static Vector4 Reflect(const Vector4& ivec, const Vector4& nvec) noexcept;
		static Vector4 Refract(const Vector4& ivec, const Vector4& nvec, float refractionIndex) noexcept;
		static Vector4 Transform(const Vector2& v, const Quaternion& q) noexcept;
		static Vector4 Transform(const Vector3& v, const Quaternion& q) noexcept;
		static Vector4 Transform(const Vector4& v, const Quaternion& q) noexcept;
		static Vector4 Transform(const Vector4& v, const Matrix& m) noexcept;

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
		explicit Matrix(_In_reads_(16) const float* array) noexcept
			: matrix_(array) {}
		constexpr Matrix(
			const DirectX::XMVECTOR& r0,
			const DirectX::XMVECTOR& r1,
			const DirectX::XMVECTOR& r2,
			const DirectX::XMVECTOR& r3) noexcept;
		constexpr Matrix(
			const Vector4& r0,
			const Vector4& r1,
			const Vector4& r2,
			const Vector4& r3) noexcept;
		~Matrix() = default;

		Matrix(const Matrix&) = default;
		Matrix& operator=(const Matrix&) = default;
		Matrix(Matrix&&) = default;
		Matrix& operator=(Matrix&&) = default;

		bool operator == (const Matrix& rhs) const noexcept;
		bool operator != (const Matrix& rhs) const noexcept;

		Matrix operator* (const Matrix& rhs) const noexcept;

		Matrix& operator*= (const Matrix& rhs) noexcept;

		const Vector4& operator [] (std::size_t index) const noexcept;
		Vector4& operator [] (std::size_t index) noexcept;

		operator DirectX::XMMATRIX() const noexcept;

		[[nodiscard]] Vector3 Right()    const noexcept;
		[[nodiscard]] Vector3 Left()     const noexcept;
		[[nodiscard]] Vector3 Up()       const noexcept;
		[[nodiscard]] Vector3 Down()     const noexcept;
		[[nodiscard]] Vector3 Forward()  const noexcept;
		[[nodiscard]] Vector3 Backward() const noexcept;

		[[nodiscard]] float Determinant() const noexcept;

		static Matrix Billboard(
			const Vector3& object, 
			const Vector3& cameraPosition,
			const Vector3& cameraUp, 
			const Vector3* cameraForward = nullptr) noexcept;
		static Matrix CreateConstrainedBillboard(
			const Vector3& object,
			const Vector3& cameraPosition, 
			const Vector3& rotateAxis,
			const Vector3* cameraForward = nullptr, 
			const Vector3* objectForward = nullptr) noexcept;
		static Matrix Transpose(const Matrix& m) noexcept;
		static Matrix Inverse(const Matrix& m) noexcept;
		static Matrix Translation(const Vector3& position) noexcept;
		static Matrix Translation(float x, float y, float z) noexcept;
		static Matrix Scale(const Vector3& scales) noexcept;
		static Matrix Scale(float xs, float ys, float zs) noexcept;
		static Matrix Scale(float scale) noexcept;
		static Matrix RotationX(float rotation) noexcept;
		static Matrix RotationY(float rotation) noexcept;
		static Matrix RotationZ(float rotation) noexcept;
		static Matrix FromAxisAngle(const Vector3& axis, float radians) noexcept;
		static Matrix PerspectiveFieldOfView(float fov, float aspectRatio, float nearPlane, float farPlane) noexcept;
		static Matrix Perspective(float width, float height, float nearPlane, float farPlane) noexcept;
		static Matrix PerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane) noexcept;
		static Matrix Orthographic(float width, float height, float zNearPlane, float zFarPlane) noexcept;
		static Matrix OrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane) noexcept;
		static Matrix LookAt(const Vector3& position, const Vector3& target, const Vector3& up) noexcept;
		static Matrix World(const Vector3& position, const Vector3& forward, const Vector3& up) noexcept;
		static Matrix FromQuaternion(const Quaternion& q) noexcept;
		static Matrix FromYawPitchRoll(float yaw, float pitch, float roll) noexcept;
		static Matrix Lerp(const Matrix& m1, const Matrix& m2, float t) noexcept;
		static Matrix Transform(const Matrix& m, const Quaternion& rotation) noexcept;

		static constexpr Matrix Identity() noexcept;

	private:
		DirectX::XMMATRIX matrix_;
	};

	class Quaternion final
	{
	public:
		constexpr Quaternion() noexcept
			: quaternion_{ 0.0f, 0.0f, 0.0f, 1.0f } {}
		constexpr Quaternion(const float x, const float y, const float z, const float w) noexcept
			: quaternion_{ x, y, z, w } {}
		explicit constexpr Quaternion(_In_reads_(4) const float* array) noexcept
			: quaternion_{ array[0], array[1], array[2], array[3] } {}
		explicit constexpr Quaternion(const DirectX::XMVECTOR& v) noexcept
			: quaternion_(v) {}
		explicit Quaternion(const Vector4& v) noexcept
			: quaternion_{ v[0], v[1], v[2], v[3] } {}
		Quaternion(const Vector3& v, const float angle) noexcept
			: quaternion_{ v[0], v[1], v[2], angle } {}
		~Quaternion() = default;

		Quaternion(const Quaternion&) = default;
		Quaternion& operator = (const Quaternion&) = default;
		Quaternion(Quaternion&&) = default;
		Quaternion& operator = (Quaternion&&) = default;

		bool operator ==(const Quaternion& rhs) const noexcept;
		bool operator != (const Quaternion& rhs) const noexcept;

		Quaternion operator + (const Quaternion& rhs) const noexcept;
		Quaternion operator - (const Quaternion& rhs) const noexcept;
		Quaternion operator * (const Quaternion& rhs) const noexcept;
		Quaternion operator * (float rhs) const noexcept;

		Quaternion& operator += (const Quaternion& rhs) noexcept;
		Quaternion& operator -= (const Quaternion& rhs) noexcept;
		Quaternion& operator *= (const Quaternion& rhs) noexcept;
		Quaternion& operator *= (float rhs) noexcept;

		operator DirectX::XMVECTOR() const noexcept;

		void Normalized() noexcept;

		[[nodiscard]] float Length() const noexcept;
		[[nodiscard]] float LengthSquared() const noexcept;

		static float Dot(const Quaternion& q1, const Quaternion& q2) noexcept;
		static Quaternion Normalize(const Quaternion& q) noexcept;
		static Quaternion Conjugate(const Quaternion& q) noexcept;
		static Quaternion Inverse(const Quaternion& q) noexcept;
		static Quaternion FromAxisAngle(const Vector3& axis, float angle) noexcept;
		static Quaternion FromYawPitchRoll(float yaw, float pitch, float roll) noexcept;
		static Quaternion FromRotationMatrix(const Matrix& m) noexcept;
		static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t) noexcept;
		static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t) noexcept;
		static Quaternion Concatenate(const Quaternion& q1, const Quaternion& q2) noexcept;

		static constexpr Quaternion Identity() noexcept { return { 0.0f, 0.0f, 0.0f, 1.0f }; }

	private:
		DirectX::XMVECTOR quaternion_;
	};

	class Color final
	{
	public:
		constexpr Color() noexcept
			: color_{ 0.0f, 0.0f ,0.0f ,1.0f } {}
		constexpr Color(const float r, const float g, const float b, const float a = 1.0f) noexcept
			: color_{ r, g, b, a } {}
		explicit constexpr Color(_In_reads_(4) const float* array) noexcept
			: color_{ array[0], array[1], array[2], array[3] } {}
		explicit Color(const DirectX::XMVECTOR& xmv) noexcept
			: color_(xmv) {}
		~Color() = default;

		Color(const Color&) = default;
		Color& operator=(const Color&) = default;
		Color(Color&&) = default;
		Color& operator=(Color&&) = default;

		bool operator == (const Color& rhs) const noexcept;
		bool operator != (const Color& rhs) const noexcept;

		constexpr Color operator + () const noexcept { return *this; }
		Color operator - () const noexcept;

		Color operator + (const Color& rhs) const noexcept;
		Color operator - (const Color& rhs) const noexcept;
		Color operator * (float rhs) const noexcept;
		Color operator / (float rhs) const noexcept;

		Color& operator += (const Color& rhs) noexcept;
		Color& operator -= (const Color& rhs) noexcept;
		Color& operator *= (float rhs) noexcept;
		Color& operator /= (float rhs) noexcept;

		operator DirectX::XMVECTOR() const noexcept;

		[[nodiscard]] float R() const noexcept { return DirectX::XMVectorGetX(color_); }
		[[nodiscard]] float G() const noexcept { return DirectX::XMVectorGetY(color_); }
		[[nodiscard]] float B() const noexcept { return DirectX::XMVectorGetZ(color_); }
		[[nodiscard]] float A() const noexcept { return DirectX::XMVectorGetW(color_); }

		void R(const float r) noexcept { color_ = DirectX::XMVectorSetX(color_, r); }
		void G(const float g) noexcept { color_ = DirectX::XMVectorSetX(color_, g); }
		void B(const float b) noexcept { color_ = DirectX::XMVectorSetX(color_, b); }
		void A(const float a) noexcept { color_ = DirectX::XMVectorSetX(color_, a); }

		static Color Negate(const Color& c) noexcept;
		static Color Saturate(const Color& c) noexcept;
		static Color Premultiply(const Color& c) noexcept;
		static Color AdjustSaturation(const Color& c, float sat) noexcept;
		static Color AdjustContrast(const Color& c, float contrast) noexcept;
		static Color Modulate(const Color& c1, const Color& c2) noexcept;
		static Color Lerp(const Color& c1, const Color& c2, float t) noexcept;

	private:
		DirectX::XMVECTOR color_;
	};

}

#include "core.inl"