/*********************************************************************
 * @file   core.h
 * @author Tomomi Murakami
 * @date   2021/10/13 0:59
 * @brief  �X�J���[�A�x�N�g���A�}�g���b�N�X���Z
 * @brief  �Q�l: https://github.com/microsoft/DirectXTK
 * @brief  �x�N�g���̃T�C�Y�͑S��16�o�C�g�Ȃ̂Œ���
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
	 * @brief  �񎟌��x�N�g���N���X(16Byte)
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
		 * @brief  X,Y�����̒l����2D�x�N�g�����\�z����
		 * @param  x X�����̒l
		 * @param  y Y�����̒l
		 */
		constexpr Vector2(const float x, const float y) noexcept
			: vector_{ x, y } {}
		/**
		 * @brief  �z�񂩂�2D�x�N�g�����\�z����
		 * @param  array �T�C�Y��2�̔z��
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

		// �R�s�[�����[�u
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
		 * @brief  2D�x�N�g���𕶎���ɕϊ�����
		 * @return �����񉻂��ꂽ2D�x�N�g��
		 */
		[[nodiscard]] std::string ToString() const noexcept
		{
			return std::format("({:6f}, {:6f})",
				DirectX::XMVectorGetX(vector_),
				DirectX::XMVectorGetY(vector_));
		}

		/**
		 * @brief  2D�x�N�g����X�����̎Q�Ƃ�Ԃ�
		 * @return X�����̒l�̎Q��
		 */
		[[nodiscard]] float& X() noexcept { return vector_.m128_f32[0]; }
		/**
		 * @brief  2D�x�N�g����Y�����̎Q�Ƃ�Ԃ�
		 * @return Y�����̒l�̎Q��
		 */
		[[nodiscard]] float& Y() noexcept { return vector_.m128_f32[1]; }

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
		 * @return 2D�x�N�g���Ԃ̋���
		 */
		static float Distance(const Vector2& v1, const Vector2& v2) noexcept;
		/**
		 * @brief  2D�x�N�g���Ԃ̋����̕��������߂�
		 * @param  v1 2D�x�N�g��
		 * @param  v2 2D�x�N�g��
		 * @return 2D�x�N�g���Ԃ̋����̕���
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
		 * @param  min �v�f�͈̔͂̍ŏ��l
		 * @param  max �v�f�͈̔͂̍ő�l
		 * @return �N�����v���ꂽ2D�x�N�g��
		 */
		static Vector2 Clamp(const Vector2& v, const float min, const float max) noexcept;
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
		 * @brief  3��2D�ʒu�x�N�g����ΐS���W��p����2D���W�x�N�g����Ԃ�
		 * @param  v1 1�Ԗڂ̏d�S�ʒu��\��2D�x�N�g��
		 * @param  v2 2�Ԗڂ̏d�S�ʒu��\��2D�x�N�g��
		 * @param  v3 3�Ԗڂ̏d�S�ʒu��\��2D�x�N�g��
		 * @param  f �d�ݕt���W��
		 * @param  g �d�ݕt���W��
		 * @return 2D���W�x�N�g��
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

	/**
	 * @brief  �O�����x�N�g���N���X(16Byte)
	 */
	class Vector3 final
	{
	public:
		/**
		 * @brief  3D�x�N�g���̑S�Ă̐�����0�ŏ���������
		 */
		constexpr Vector3() noexcept
			: vector_{} {}
		/**
		 * @brief  X,Y,Z�����̒l����3D�x�N�g�����\�z����
		 * @param  x X�����̒l
		 * @param  y Y�����̒l
		 * @param  z Z�����̒l
		 */
		constexpr Vector3(const float x, const float y, const float z) noexcept
			: vector_{ x, y, z } {}
		/**
		 * @brief  �z�񂩂�3D�x�N�g�����\�z����
		 * @param  array �T�C�Y��3�̔z��
		 */
		explicit constexpr Vector3(_In_reads_(3) const float* array) noexcept
			: vector_{ array[0], array[1], array[2] } {}
		/**
		 * @brief  �S�Ă̗v�f�������l��3D�x�N�g�����\�z����
		 * @param  f �v�f�̒l
		 */
		explicit constexpr Vector3(const float f) noexcept
			: vector_{ f, f, f } {}
		/**
		 * @brief  XMVECTOR�^����3D�x�N�g�����\�z����
		 * @param  xmv XMVECTOR
		 */
		explicit constexpr Vector3(const DirectX::XMVECTOR& xmv) noexcept
			: vector_{ xmv.m128_f32[0], xmv.m128_f32[1], xmv.m128_f32[2] } {}
		/**
		 * @brief  XMFLOAT3�^����3D�x�N�g�����\�z����
		 * @param  xmf XMFLOAT
		 */
		explicit Vector3(const DirectX::XMFLOAT3& xmf) noexcept
			: vector_(DirectX::XMLoadFloat3(&xmf)) {}

		~Vector3() = default;

		// �R�s�[�����[�u
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

		// �Y�����Z�q
		const float& operator [] (std::size_t index) const noexcept;
		float& operator [] (std::size_t index) noexcept;

		// �L���X�g���Z�q
		operator DirectX::XMVECTOR() const noexcept;
		operator DirectX::XMFLOAT3() const noexcept;

		/**
		 * @brief  3D�x�N�g���𕶎���ɕϊ�����
		 * @return �����񉻂��ꂽ3D�x�N�g��
		 */
		[[nodiscard]] std::string ToString() const noexcept
		{
			return std::format("({:6f}, {:6f}, {:6f})",
				DirectX::XMVectorGetX(vector_),
				DirectX::XMVectorGetY(vector_),
				DirectX::XMVectorGetZ(vector_));
		}

		/**
		 * @brief  3D�x�N�g����X�����̎Q�Ƃ��擾����
		 * @return X�����̒l�̎Q��
		 */
		[[nodiscard]] float& X() noexcept { return vector_.m128_f32[0]; }
		/**
		 * @brief  3D�x�N�g����Y�����̎Q�Ƃ��擾����
		 * @return Y�����̒l�̎Q��
		 */
		[[nodiscard]] float& Y() noexcept { return vector_.m128_f32[1]; }
		/**
		 * @brief  3D�x�N�g����Z�����̎Q�Ƃ��擾����
		 * @return Z�����̒l�̎Q��
		 */
		[[nodiscard]] float& Z() noexcept { return vector_.m128_f32[2]; }

		/**
		 * @brief  3D�x�N�g���𐳋K������
		 */
		void Normalized() noexcept;

		/**
		 * @brief  3D�x�N�g���̒��������߂�
		 * @return 3D�x�N�g���̒���
		 */
		[[nodiscard]] float Length() const noexcept;
		/**
		 * @brief  3D�x�N�g���̒����̕��������߂�
		 * @return 3D�x�N�g���̒����̕���
		 */
		[[nodiscard]] float LengthSquared() const noexcept;

		/**
		 * @param   v �e�X�g����3D�x�N�g��
		 * @param   bounds ���E�����߂�3D�x�N�g��
		 * @return  3D�x�N�g���̐������ݒ肵���͈͓��ɂ��邩
		 * @note https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvector3inbounds
		 */
		static bool InBounds(const Vector3& v, const Vector3& bounds) noexcept;
		/**
		 * @brief  3D�x�N�g���Ԃ̋��������߂�
		 * @param  v1 3D�x�N�g��
		 * @param  v2 3D�x�N�g��
		 * @return 3D�x�N�g���Ԃ̋���
		 */
		static float Distance(const Vector3& v1, const Vector3& v2) noexcept;
		/**
		 * @brief  3D�x�N�g���Ԃ̋����̕��������߂�
		 * @param  v1 3D�x�N�g��
		 * @param  v2 3D�x�N�g��
		 * @return 3D�x�N�g���Ԃ̋����̕���
		 */
		static float DistanceSquared(const Vector3& v1, const Vector3& v2) noexcept;
		/**
		 * @brief  3D�x�N�g���̓��ς����߂�
		 * @param  v1 3D�x�N�g��
		 * @param  v2 3D�x�N�g��
		 * @return 3D�x�N�g���̓���
		 */
		static float Dot(const Vector3& v1, const Vector3& v2) noexcept;
		/**
		 * @brief  3D�x�N�g���̊O�ς����߂�
		 * @param  v1 3D�x�N�g��
		 * @param  v2 3D�x�N�g��
		 * @return 3D�x�N�g���̊O��
		 */
		static Vector3 Cross(const Vector3& v1, const Vector3& v2) noexcept;
		/**
		 * @brief  3D�x�N�g���𐳋K������
		 * @param  v 3D�x�N�g��
		 * @return ���K�����ꂽ3D�x�N�g��
		 */
		static Vector3 Normalize(const Vector3& v) noexcept;
		/**
		 * @brief  3D�x�N�g���̐������w�肵���ŏ��l�ƍő�l�͈̔͂ɃN�����v����
		 * @param  v �N�����v����鐬��������3D�x�N�g��
		 * @param  min �v�f�͈̔͂̍ŏ��l
		 * @param  max �v�f�͈̔͂̍ő�l
		 * @return �N�����v���ꂽ3D�x�N�g��
		 */
		static Vector3 Clamp(const Vector3& v, const float min, const float max) noexcept;
		/**
		 * @brief  3D�x�N�g���̐������w�肵���ŏ��l�ƍő�l�͈̔͂ɃN�����v����
		 * @param  v �N�����v����鐬��������3D�x�N�g��
		 * @param  vmin �ŏ��͈͂̐���������3D�x�N�g��
		 * @param  vmax �ő�͈͂̐���������3D�x�N�g��
		 * @return �N�����v���ꂽ3D�x�N�g��
		 */
		static Vector3 Clamp(const Vector3& v, const Vector3& vmin, const Vector3& vmax) noexcept;
		/**
		 * @brief  2��3D�x�N�g���̐������Ƃ̔�r���s���A�ŏ��̐���������3D�x�N�g����Ԃ�
		 * @param  v1 3D�x�N�g��
		 * @param  v2 3D�x�N�g��
		 * @return �ŏ��̐�����������3D�x�N�g��
		 */
		static Vector3 Min(const Vector3& v1, const Vector3& v2) noexcept;
		/**
		 * @brief  2��3D�x�N�g���̐������Ƃ̔�r���s���A�ő�̐���������3D�x�N�g����Ԃ�
		 * @param  v1 3D�x�N�g��
		 * @param  v2 3D�x�N�g��
		 * @return �ő�̐�����������3D�x�N�g��
		 */
		static Vector3 Max(const Vector3& v1, const Vector3& v2) noexcept;
		/**
		 * @brief  3D�x�N�g���Ԃ̐��`��Ԃ��s��
		 * @param  v1 3D�n�_�x�N�g��
		 * @param  v2 3D�I�_�x�N�g��
		 * @param  t ��Ԑ���W��
		 * @return �⊮���ʂ�\��3D�x�N�g��
		 */
		static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) noexcept;
		/**
		 * @brief  3D�x�N�g���Ԃ��G���~�[�g��������p���Đ��`��Ԃ��s��
		 * @param  v1 3D�n�_�x�N�g��
		 * @param  v2 3D�I�_�x�N�g��
		 * @param  t ��Ԑ���W��
		 * @return �⊮���ʂ�\��3D�x�N�g��
		 */
		static Vector3 SmoothStep(const Vector3& v1, const Vector3& v2, float t) noexcept;
		/**
		 * @brief  3��3D�ʒu�x�N�g����ΐS���W��p����3D���W�x�N�g����Ԃ�
		 * @param  v1 1�Ԗڂ̏d�S�ʒu��\��3D�x�N�g��
		 * @param  v2 2�Ԗڂ̏d�S�ʒu��\��3D�x�N�g��
		 * @param  v3 3�Ԗڂ̏d�S�ʒu��\��3D�x�N�g��
		 * @param  f �d�ݕt���W��
		 * @param  g �d�ݕt���W��
		 * @return 3D���W�x�N�g��
		 * @note   https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvectorbarycentric
		 */
		static Vector3 BaryCentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g) noexcept;
		/**
		 * @brief  �w�肳�ꂽ3D�ʒu�x�N�g����p���ăJ�b�g���������X�v���C���Ȑ���Ԃ��s��
		 * @param  v1 1�ڂ̐���_��\��3D�x�N�g��
		 * @param  v2 2�ڂ̐���_��\��3D�x�N�g��
		 * @param  v3 3�ڂ̐���_��\��3D�x�N�g��
		 * @param  v4 4�ڂ̐���_��\��3D�x�N�g��
		 * @param  t ��Ԑ���W��
		 * @return �⊮���ʂ�\��3D�x�N�g��
		 * @note   https://docs.microsoft.com/ja-jp/windows/win32/api/directxmath/nf-directxmath-xmquaternionbarycentric
		 */
		static Vector3 CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t) noexcept;
		/**
		 * @brief  �w�肳�ꂽ3D�ʒu�x�N�g����p���ăG���~�[�g�Ȑ���Ԃ��s��
		 * @param  v1 1�ڂ̐���_�̈ʒu��\��3D�x�N�g��
		 * @param  t1 1�ڂ̐���_�̑��x��\��3D�x�N�g��
		 * @param  v2 2�ڂ̐���_�̈ʒu��\��3D�x�N�g��
		 * @param  t2 2�ڂ̐���_�̑��x��\��3D�x�N�g��
		 * @param  t ��Ԑ���W��
		 * @return �⊮���ʂ�\��3D�x�N�g��
		 * @note   https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvectorcatmullrom
		 */
		static Vector3 Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t) noexcept;
		/**
		 * @brief  3D���˃x�N�g����3D�@���x�N�g���Ŕ��˂�����3D�x�N�g�������߂�
		 * @param  ivec 3D���˃x�N�g��
		 * @param  nvec 3D�@���x�N�g��
		 * @return 3D���˃x�N�g��
		 * @note   https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvectorhermite
		 */
		static Vector3 Reflect(const Vector3& ivec, const Vector3& nvec) noexcept;
		/**
		 * @brief  3D���˃x�N�g����3D�@���x�N�g���ŋ��܂�����3D�x�N�g�������߂�
		 * @param  ivec 3D���˃x�N�g��
		 * @param  nvec 3D�@���x�N�g��
		 * @param  refractionIndex ���ܗ�
		 * @return 3D���܃x�N�g��
		 */
		static Vector3 Refract(const Vector3& ivec, const Vector3& nvec, float refractionIndex) noexcept;
		/**
		 * @brief  �N�H�[�^�j�I�����g�p����3D�x�N�g������]����
		 * @param  v ��]����3D�x�N�g��
		 * @param  q 3D�x�N�g���ɉ�]��K������N�H�[�^�j�I��
		 * @return ��]����3D�x�N�g��
		 */
		static Vector3 Transform(const Vector3& v, const Quaternion& q) noexcept;
		/**
		 * @brief  �ϊ��s����g�p����3D�x�N�g����ϊ�����
		 * @param  v �ϊ�����3D�x�N�g��
		 * @param  m �ϊ��s��
		 * @return �ϊ����ꂽ3D�x�N�g��
		 */
		static Vector3 Transform(const Vector3& v, const Matrix& m) noexcept;
		/**
		 * @brief  �ϊ��s����g�p����3D�@���x�N�g����ϊ�����
		 * @param  v �ϊ�����3D�@���x�N�g��
		 * @param  m �ϊ��s��
		 * @return �ϊ����ꂽ3D�@���x�N�g��
		 */
		static Vector3 TransformNormal(const Vector3& v, const Matrix& m) noexcept;

		/**
		 * @brief  �S�Ă̐�����0�ł���3D�x�N�g����Ԃ�
		 * @return �S�Ă̐�����0��3D�x�N�g��
		 */
		static constexpr Vector3 Zero() noexcept { return{  0.0f,  0.0f,  0.0f }; }
		/**
		 * @brief  �S�Ă̐�����1�ł���3D�x�N�g����Ԃ�
		 * @return �S�Ă̐�����1��3D�x�N�g��
		 */
		static constexpr Vector3 One() noexcept { return{  1.0f,  1.0f,  1.0f }; }
		/**
		 * @brief  �E����(X+)��3D�P�ʃx�N�g����Ԃ�
		 * @return �E������3D�P�ʃx�N�g��
		 */
		static constexpr Vector3 Right() noexcept { return{  1.0f,  0.0f,  0.0f }; }
		/**
		 * @brief  ������(X-)��3D�P�ʃx�N�g����Ԃ�
		 * @return ��������3D�P�ʃx�N�g��
		 */
		static constexpr Vector3 Left() noexcept { return{ -1.0f,  0.0f,  0.0f }; }
		/**
		 * @brief  �����(Y+)��3D�P�ʃx�N�g����Ԃ�
		 * @return �������3D�P�ʃx�N�g��
		 */
		static constexpr Vector3 Up() noexcept { return{  0.0f,  1.0f,  0.0f }; }
		/**
		 * @brief  ������(Y-)��3D�P�ʃx�N�g����Ԃ�
		 * @return ��������3D�P�ʃx�N�g��
		 */
		static constexpr Vector3 Down() noexcept { return{  0.0f, -1.0f,  0.0f }; }
		/**
		 * @brief  �O����(Z+)��3D�P�ʃx�N�g����Ԃ�
		 * @return �O������3D�P�ʃx�N�g��
		 */
		static constexpr Vector3 Forward() noexcept { return{  0.0f,  0.0f,  1.0f }; }
		/**
		 * @brief  ������(Z-)��3D�P�ʃx�N�g����Ԃ�
		 * @return ��������3D�P�ʃx�N�g��
		 */
		static constexpr Vector3 Backward() noexcept { return{  0.0f,  0.0f, -1.0f }; }

	private:
		DirectX::XMVECTOR vector_;
	};

	class Vector4 final
	{
	public:
		/**
		 * @brief  4D�x�N�g���̑S�Ă̐�����0�ŏ���������
		 */
		constexpr Vector4() noexcept
			: vector_{} {}
		/**
		 * @brief  X,Y,Z,W�����̒l����4D�x�N�g�����\�z����
		 * @param  x X�����̒l
		 * @param  y Y�����̒l
		 * @param  z Z�����̒l
		 * @param  w W�����̒l
		 */
		constexpr Vector4(const float x, const float y, const float z, const float w) noexcept
			: vector_{ x, y, z, w } {}
		/**
		 * @brief  �z�񂩂�4D�x�N�g�����\�z����
		 * @param  array �T�C�Y��4�̔z��
		 */
		explicit constexpr Vector4(_In_reads_(4) const float* array) noexcept
			: vector_{ array[0], array[1], array[2], array[3] } {}
		/**
		 * @brief  �S�Ă̗v�f�������l��4D�x�N�g�����\�z����
		 * @param  f �v�f�̒l
		 */
		explicit constexpr Vector4(const float f) noexcept
			: vector_{ f, f, f, f } {}
		/**
		 * @brief  XMVECTOR�^����4D�x�N�g�����\�z����
		 * @param  xmv XMVECTOR
		 */
		explicit constexpr Vector4(const DirectX::XMVECTOR& xmv) noexcept
			: vector_(xmv) {}
		/**
		 * @brief  XMFLOAT4�^����4D�x�N�g�����\�z����
		 * @param  xmf XMFLOAT
		 */
		explicit Vector4(const DirectX::XMFLOAT4& xmf) noexcept
			: vector_(DirectX::XMLoadFloat4(&xmf)) {}

		~Vector4() = default;

		// �R�s�[�����[�u
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

		// �Y�����Z�q
		const float& operator [] (std::size_t index) const noexcept;
		float& operator [] (std::size_t index) noexcept;

		// �L���X�g���Z�q
		operator DirectX::XMVECTOR() const noexcept;
		operator DirectX::XMFLOAT4() const noexcept;

		/**
		 * @brief  4D�x�N�g���𕶎���ɕϊ�����
		 * @return �����񉻂��ꂽ4D�x�N�g��
		 */
		[[nodiscard]] std::string ToString() const noexcept
		{
			return std::format("({:6f}, {:6f}, {:6f}, {:6f})",
				DirectX::XMVectorGetX(vector_),
				DirectX::XMVectorGetY(vector_),
				DirectX::XMVectorGetZ(vector_),
				DirectX::XMVectorGetW(vector_));
		}

		/**
		 * @brief  4D�x�N�g����X�����̎Q�Ƃ��擾����
		 * @return X�����̒l�̎Q��
		 */
		[[nodiscard]] float& X() noexcept { return vector_.m128_f32[0]; }
		/**
		 * @brief  4D�x�N�g����Y�����̎Q�Ƃ��擾����
		 * @return Y�����̒l�̎Q��
		 */
		[[nodiscard]] float& Y() noexcept { return vector_.m128_f32[1]; }
		/**
		 * @brief  4D�x�N�g����Z�����̎Q�Ƃ��擾����
		 * @return Z�����̒l�̎Q��
		 */
		[[nodiscard]] float& Z() noexcept { return vector_.m128_f32[2]; }
		/**
		 * @brief  4D�x�N�g����W�����̎Q�Ƃ��擾����
		 * @return W�����̒l�̎Q��
		 */
		[[nodiscard]] float& W() noexcept { return vector_.m128_f32[3]; }

		/**
		 * @brief  4D�x�N�g���𐳋K������
		 */
		void Normalized() noexcept;

		/**
		 * @brief  4D�x�N�g���̒��������߂�
		 * @return 4D�x�N�g���̒���
		 */
		[[nodiscard]] float Length() const noexcept;
		/**
		 * @brief  4D�x�N�g���̒����̕��������߂�
		 * @return 4D�x�N�g���̒����̕���
		 */
		[[nodiscard]] float LengthSquared() const noexcept;

		/**
		 * @param   v �e�X�g����4D�x�N�g��
		 * @param   bounds ���E�����߂�4D�x�N�g��
		 * @return  4D�x�N�g���̐������ݒ肵���͈͓��ɂ��邩
		 * @note https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvector4inbounds
		 */
		static bool InBounds(const Vector4& v, const Vector4& bounds) noexcept;
		/**
		 * @brief  4D�x�N�g���Ԃ̋��������߂�
		 * @param  v1 4D�x�N�g��
		 * @param  v2 4D�x�N�g��
		 * @return 4D�x�N�g���Ԃ̋���
		 */
		static float Distance(const Vector4& v1, const Vector4& v2) noexcept;
		/**
		 * @brief  4D�x�N�g���Ԃ̋����̕��������߂�
		 * @param  v1 4D�x�N�g��
		 * @param  v2 4D�x�N�g��
		 * @return 4D�x�N�g���Ԃ̋����̕���
		 */
		static float DistanceSquared(const Vector4& v1, const Vector4& v2) noexcept;
		/**
		 * @brief  4D�x�N�g���̓��ς����߂�
		 * @param  v1 4D�x�N�g��
		 * @param  v2 4D�x�N�g��
		 * @return 4D�x�N�g���̓���
		 */
		static float Dot(const Vector4& v1, const Vector4& v2) noexcept;
		/**
		 * @brief  4D�x�N�g���̊O�ς����߂�
		 * @param  v1 4D�x�N�g��
		 * @param  v2 4D�x�N�g��
		 * @param  v3 4D�x�N�g��
		 * @return 4D�x�N�g���̊O��
		 */
		static Vector4 Cross(const Vector4& v1, const Vector4& v2, const Vector4& v3) noexcept;
		/**
		 * @brief  4D�x�N�g���𐳋K������
		 * @param  v 4D�x�N�g��
		 * @return ���K�����ꂽ4D�x�N�g��
		 */
		static Vector4 Normalize(const Vector4& v) noexcept;
		/**
		 * @brief  4D�x�N�g���̐������w�肵���ŏ��l�ƍő�l�͈̔͂ɃN�����v����
		 * @param  v �N�����v����鐬��������4D�x�N�g��
		 * @param  min �v�f�͈̔͂̍ŏ��l
		 * @param  max �v�f�͈̔͂̍ő�l
		 * @return �N�����v���ꂽ4D�x�N�g��
		 */
		static Vector4 Clamp(const Vector4& v, const float min, const float max) noexcept;
		/**
		 * @brief  4D�x�N�g���̐������w�肵���ŏ��l�ƍő�l�͈̔͂ɃN�����v����
		 * @param  v �N�����v����鐬��������4D�x�N�g��
		 * @param  vmin �ŏ��͈͂̐���������4D�x�N�g��
		 * @param  vmax �ő�͈͂̐���������4D�x�N�g��
		 * @return �N�����v���ꂽ4D�x�N�g��
		 */
		static Vector4 Clamp(const Vector4& v, const Vector4& vmin, const Vector4& vmax) noexcept;
		/**
		 * @brief  2��4D�x�N�g���̐������Ƃ̔�r���s���A�ŏ��̐���������4D�x�N�g����Ԃ�
		 * @param  v1 4D�x�N�g��
		 * @param  v2 4D�x�N�g��
		 * @return �ŏ��̐�����������4D�x�N�g��
		 */
		static Vector4 Min(const Vector4& v1, const Vector4& v2) noexcept;
		/**
		 * @brief  2��4D�x�N�g���̐������Ƃ̔�r���s���A�ő�̐���������4D�x�N�g����Ԃ�
		 * @param  v1 4D�x�N�g��
		 * @param  v2 4D�x�N�g��
		 * @return �ő�̐�����������4D�x�N�g��
		 */
		static Vector4 Max(const Vector4& v1, const Vector4& v2) noexcept;
		/**
		 * @brief  4D�x�N�g���Ԃ̐��`��Ԃ��s��
		 * @param  v1 4D�n�_�x�N�g��
		 * @param  v2 4D�I�_�x�N�g��
		 * @param  t ��Ԑ���W��
		 * @return �⊮���ʂ�\��4D�x�N�g��
		 */
		static Vector4 Lerp(const Vector4& v1, const Vector4& v2, float t) noexcept;
		/**
		 * @brief  4D�x�N�g���Ԃ��G���~�[�g��������p���Đ��`��Ԃ��s��
		 * @param  v1 4D�n�_�x�N�g��
		 * @param  v2 4D�I�_�x�N�g��
		 * @param  t ��Ԑ���W��
		 * @return �⊮���ʂ�\��4D�x�N�g��
		 */
		static Vector4 SmoothStep(const Vector4& v1, const Vector4& v2, float t) noexcept;
		/**
		 * @brief  3��4D�ʒu�x�N�g����ΐS���W��p����4D���W�x�N�g����Ԃ�
		 * @param  v1 1�Ԗڂ̏d�S�ʒu��\��4D�x�N�g��
		 * @param  v2 2�Ԗڂ̏d�S�ʒu��\��4D�x�N�g��
		 * @param  v3 3�Ԗڂ̏d�S�ʒu��\��4D�x�N�g��
		 * @param  f �d�ݕt���W��
		 * @param  g �d�ݕt���W��
		 * @return 4D���W�x�N�g��
		 * @note   https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvectorbarycentric
		 */
		static Vector4 BaryCentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g) noexcept;
		/**
		 * @brief  �w�肳�ꂽ4D�ʒu�x�N�g����p���ăJ�b�g���������X�v���C���Ȑ���Ԃ��s��
		 * @param  v1 1�ڂ̐���_��\��4D�x�N�g��
		 * @param  v2 2�ڂ̐���_��\��4D�x�N�g��
		 * @param  v3 3�ڂ̐���_��\��4D�x�N�g��
		 * @param  v4 4�ڂ̐���_��\��4D�x�N�g��
		 * @param  t ��Ԑ���W��
		 * @return �⊮���ʂ�\��4D�x�N�g��
		 * @note   https://docs.microsoft.com/ja-jp/windows/win32/api/directxmath/nf-directxmath-xmquaternionbarycentric
		 */
		static Vector4 CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t) noexcept;
		/**
		 * @brief  �w�肳�ꂽ4D�ʒu�x�N�g����p���ăG���~�[�g�Ȑ���Ԃ��s��
		 * @param  v1 1�ڂ̐���_�̈ʒu��\��4D�x�N�g��
		 * @param  t1 1�ڂ̐���_�̑��x��\��4D�x�N�g��
		 * @param  v2 2�ڂ̐���_�̈ʒu��\��4D�x�N�g��
		 * @param  t2 2�ڂ̐���_�̑��x��\��4D�x�N�g��
		 * @param  t ��Ԑ���W��
		 * @return �⊮���ʂ�\��4D�x�N�g��
		 * @note   https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvectorcatmullrom
		 */
		static Vector4 Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t) noexcept;
		/**
		 * @brief  4D���˃x�N�g����4D�@���x�N�g���Ŕ��˂�����4D�x�N�g�������߂�
		 * @param  ivec 4D���˃x�N�g��
		 * @param  nvec 4D�@���x�N�g��
		 * @return4D ���˃x�N�g��
		 * @note   https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvectorhermite
		 */
		static Vector4 Reflect(const Vector4& ivec, const Vector4& nvec) noexcept;
		/**
		 * @brief  4D���˃x�N�g����4D�@���x�N�g���ŋ��܂�����4D�x�N�g�������߂�
		 * @param  ivec 4D���˃x�N�g��
		 * @param  nvec 4D�@���x�N�g��
		 * @param  refractionIndex ���ܗ�
		 * @return 4D���܃x�N�g��
		 */
		static Vector4 Refract(const Vector4& ivec, const Vector4& nvec, float refractionIndex) noexcept;
		/**
		 * @brief  �N�H�[�^�j�I�����g�p����4D�x�N�g������]����
		 * @param  v ��]����4D�x�N�g��
		 * @param  q 4D�x�N�g���ɉ�]��K������N�H�[�^�j�I��
		 * @return ��]����4D�x�N�g��
		 */
		static Vector4 Transform(const Vector2& v, const Quaternion& q) noexcept;
		/**
		 * @brief  �N�H�[�^�j�I�����g�p����2D�x�N�g������]����
		 * @param  v ��]����2D�x�N�g��
		 * @param  q 2D�x�N�g���ɉ�]��K������N�H�[�^�j�I��
		 * @return ��]����4D�x�N�g��
		 */
		static Vector4 Transform(const Vector3& v, const Quaternion& q) noexcept;
		/**
		 * @brief  �N�H�[�^�j�I�����g�p����3D�x�N�g������]����
		 * @param  v ��]����3D�x�N�g��
		 * @param  q 3D�x�N�g���ɉ�]��K������N�H�[�^�j�I��
		 * @return ��]����4D�x�N�g��
		 */
		static Vector4 Transform(const Vector4& v, const Quaternion& q) noexcept;
		/**
		 * @brief  �ϊ��s����g�p����4D�x�N�g����ϊ�����
		 * @param  v �ϊ�����4D�x�N�g��
		 * @param  m �ϊ��s��
		 * @return �ϊ����ꂽ4D�x�N�g��
		 */
		static Vector4 Transform(const Vector4& v, const Matrix& m) noexcept;

		/**
		 * @brief  �S�Ă̐�����0�ł���4D�x�N�g����Ԃ�
		 * @return �S�Ă̐�����0��4D�x�N�g��
		 */
		static constexpr Vector4 Zero() noexcept { return{ 0.0f, 0.0f, 0.0f, 0.0f }; }
		/**
		 * @brief  �S�Ă̐�����1�ł���4D�x�N�g����Ԃ�
		 * @return �S�Ă̐�����1��4D�x�N�g��
		 */
		static constexpr Vector4 One() noexcept { return{ 1.0f, 1.0f, 1.0f, 1.0f }; }
		/**
		 * @brief  X������1��4D�P�ʃx�N�g����Ԃ�
		 * @return X������1��4D�P�ʃx�N�g��
		 */
		static constexpr Vector4 UnitX() noexcept { return{ 1.0f, 0.0f, 0.0f, 0.0f }; }
		/**
		 * @brief  Y������1��4D�P�ʃx�N�g����Ԃ�
		 * @return Y������1��4D�P�ʃx�N�g��
		 */
		static constexpr Vector4 UnitY() noexcept { return{ 0.0f, 1.0f, 0.0f, 0.0f }; }
		/**
		 * @brief  Z������1��4D�P�ʃx�N�g����Ԃ�
		 * @return Z������1��4D�P�ʃx�N�g��
		 */
		static constexpr Vector4 UnitZ() noexcept { return{ 0.0f, 0.0f, 1.0f, 0.0f }; }
		/**
		 * @brief  W����������1��4D�P�ʃx�N�g����Ԃ�
		 * @return W������1��4D�P�ʃx�N�g��
		 */
		static constexpr Vector4 UnitW() noexcept { return{ 0.0f, 0.0f, 0.0f, 1.0f }; }

	private:
		DirectX::XMVECTOR vector_;
	};

	/**
	 * @brief  �񎟌��x�N�g���N���X(64Byte)
	 */
	class Matrix final
	{
	public:
		/**
		 * @brief  �P�ʍs����\�z����
		 */
		constexpr Matrix() noexcept;
		/**
		 * @brief  �l����s����\�z����
		 * @param  m00 (0, 0)�v�f�̒l
		 * @param  m01 (0, 1)�v�f�̒l
		 * @param  m02 (0, 2)�v�f�̒l
		 * @param  m03 (0, 3)�v�f�̒l
		 * @param  m10 (1, 0)�v�f�̒l
		 * @param  m11 (1, 1)�v�f�̒l
		 * @param  m12 (1, 2)�v�f�̒l
		 * @param  m13 (1, 3)�v�f�̒l
		 * @param  m20 (2, 0)�v�f�̒l
		 * @param  m21 (2, 1)�v�f�̒l
		 * @param  m22 (2, 2)�v�f�̒l
		 * @param  m23 (2, 3)�v�f�̒l
		 * @param  m30 (3, 0)�v�f�̒l
		 * @param  m31 (3, 1)�v�f�̒l
		 * @param  m32 (3, 2)�v�f�̒l
		 * @param  m33 (3, 3)�v�f�̒l
		 */
		constexpr Matrix(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33) noexcept;
		/**
		 * @brief  �z�񂩂�s����\�z����
		 * @param  array �T�C�Y��16�̔z��
		 */
		explicit Matrix(_In_reads_(16) const float* array) noexcept
			: matrix_(array) {}
		/**
		 * @brief  XMMATRIX�^����s����\�z����
		 * @param  xmm XMMATRIX
		 */
		explicit constexpr Matrix(const DirectX::XMMATRIX& xmm) noexcept
			: matrix_(xmm) {}
		/**
		 * @brief  4��XMVECTOR�^����s����\�z����
		 * @param  r0 1���
		 * @param  r1 2���
		 * @param  r2 3���
		 * @param  r3 4���
		 */
		constexpr Matrix(
			const DirectX::XMVECTOR& r0,
			const DirectX::XMVECTOR& r1,
			const DirectX::XMVECTOR& r2,
			const DirectX::XMVECTOR& r3) noexcept;
		/**
		 * @brief  4��4D�x�N�g������s����\�z����
		 * @param  r0 1���
		 * @param  r1 2���
		 * @param  r2 3���
		 * @param  r3 4���
		 */
		constexpr Matrix(
			const Vector4& r0,
			const Vector4& r1,
			const Vector4& r2,
			const Vector4& r3) noexcept;

		~Matrix() = default;

		// �R�s�[&���[�u
		Matrix(const Matrix&) = default;
		Matrix& operator=(const Matrix&) = default;
		Matrix(Matrix&&) = default;
		Matrix& operator=(Matrix&&) = default;

		// ��r���Z�q
		bool operator == (const Matrix& rhs) const noexcept;
		bool operator != (const Matrix& rhs) const noexcept;

		// �񍀉��Z�q
		Matrix operator* (const Matrix& rhs) const noexcept;

		// ������Z�q
		Matrix& operator*= (const Matrix& rhs) noexcept;

		// �Y�����Z�q
		const Vector4& operator [] (std::size_t index) const noexcept;
		Vector4& operator [] (std::size_t index) noexcept;

		// �L���X�g���Z�q
		operator DirectX::XMMATRIX() const noexcept;
		operator DirectX::XMFLOAT4X4() const noexcept;

		/**
		 * @brief  �s��̉E�������w���P�ʃx�N�g�������߂�
		 * @return �E�����x�N�g��
		 */
		[[nodiscard]] Vector3 Right()    const noexcept;
		/**
		 * @brief  �s��̍��������w���P�ʃx�N�g�������߂�
		 * @return �������x�N�g��
		 */
		[[nodiscard]] Vector3 Left()     const noexcept;
		/**
		 * @brief  �s��̏�������w���P�ʃx�N�g�������߂�
		 * @return ������x�N�g��
		 */
		[[nodiscard]] Vector3 Up()       const noexcept;
		/**
		 * @brief  �s��̉��������w���P�ʃx�N�g�������߂�
		 * @return �������x�N�g��
		 */
		[[nodiscard]] Vector3 Down()     const noexcept;
		/**
		 * @brief  �s��̑O�������w���P�ʃx�N�g�������߂�
		 * @return �O�����x�N�g��
		 */
		[[nodiscard]] Vector3 Forward()  const noexcept;
		/**
		 * @brief  �s��̌��������w���P�ʃx�N�g�������߂�
		 * @return �������x�N�g��
		 */
		[[nodiscard]] Vector3 Backward() const noexcept;

		/**
		 * @brief  �s�񎮂��v�Z����
		 * @return �v�Z����
		 */
		[[nodiscard]] float Determinant() const noexcept;

		/**
		 * @brief  �r���{�[�h�s������߂�
		 * @param  object �r���{�[�h�̉�]�̒��S���W��\��3D�x�N�g��
		 * @param  cameraPosition �J�����̈ʒu��\��3D�x�N�g��
		 * @param  cameraUp �J�����̏������\��3D�x�N�g��
		 * @param  cameraForward �J�����̑O������\��3D�x�N�g��
		 * @return �r���{�[�h�s��
		 */
		static Matrix Billboard(
			const Vector3& object, 
			const Vector3& cameraPosition,
			const Vector3& cameraUp, 
			const Vector3* cameraForward = nullptr) noexcept;
		/**
		 * @brief  �w�肵�����𒆐S�ɉ�]����~���r���{�[�h�s������߂�
		 * @param  object �r���{�[�h�̉�]�̒��S���W��\��3D�x�N�g��
		 * @param  cameraPosition �J�����̈ʒu��\��3D�x�N�g��
		 * @param  rotateAxis �r���{�[�h�̉�]����\��3D�x�N�g��
		 * @param  cameraForward �J�����̑O������\��3D�x�N�g��
		 * @param  objectForward �I�u�W�F�N�g�̑O������\��3D�x�N�g��
		 * @return �r���{�[�h�s��
		 */
		static Matrix CreateConstrainedBillboard(
			const Vector3& object,
			const Vector3& cameraPosition, 
			const Vector3& rotateAxis,
			const Vector3* cameraForward = nullptr, 
			const Vector3* objectForward = nullptr) noexcept;
		/**
		 * @brief  �]�u�s������߂�
		 * @param  m �s��
		 * @return �]�u�s��
		 */
		static Matrix Transpose(const Matrix& m) noexcept;
		/**
		 * @brief  �t�s������߂�
		 * @param  m �s��
		 * @return �t�s��
		 */
		static Matrix Inverse(const Matrix& m) noexcept;
		/**
		 * @brief  3D�x�N�g�����畽�s�ړ��s����쐬����
		 * @param  position ���s�ړ���\��3D�x�N�g��
		 * @return ���s�ړ��s��
		 */
		static Matrix Translation(const Vector3& position) noexcept;
		/**
		 * @brief  X,Y,Z���̈ړ��ʂ��畽�s�ړ��s����쐬����
		 * @param x X�������̈ړ���
		 * @param y Y�������̈ړ���
		 * @param z Z�������̈ړ���
		 * @return ���s�ړ��s��
		 */
		static Matrix Translation(float x, float y, float z) noexcept;
		/**
		 * @brief  3D�x�N�g������g��k���s����쐬����
		 * @param  scales �g��k����\��3D�x�N�g��
		 * @return �g��k���s��
		 */
		static Matrix Scale(const Vector3& scales) noexcept;
		/**
		 * @brief  X,Y,Z���̊g�嗦����g��k���s����쐬����
		 * @param  xs X���̊g�嗦
		 * @param  ys Y���̊g�嗦
		 * @param  zs Z���̊g�嗦
		 * @return �g��k���s��
		 */
		static Matrix Scale(float xs, float ys, float zs) noexcept;
		/**
		 * @brief  �g�嗦����g��k���s����쐬����
		 * @param  scale �g�嗦
		 * @return �g��k���s��
		 */
		static Matrix Scale(float scale) noexcept;
		/**
		 * @brief  X���ŉ�]��������]�s����쐬����
		 * @param  rotation X���̃��W�A���p�x
		 * @return ��]�s��
		 */
		static Matrix RotationX(float rotation) noexcept;
		/**
		 * @brief  Y���ŉ�]��������]�s����쐬����
		 * @param  rotation Y���̃��W�A���p�x
		 * @return ��]�s��
		 */
		static Matrix RotationY(float rotation) noexcept;
		/**
		 * @brief  Z���ŉ�]��������]�s����쐬����
		 * @param  rotation Z���̃��W�A���p�x
		 * @return ��]�s��
		 */
		static Matrix RotationZ(float rotation) noexcept;
		/**
		 * @brief  3D�x�N�g�������ɉ�]��������]�s����쐬����
		 * @param  axis ��]����\��3D�x�N�g��
		 * @param  radians ���̃��W�A���p�x
		 * @return ��]�s��
		 */
		static Matrix RotationAxisAngle(const Vector3& axis, float radians) noexcept;
		static Matrix RotationFromQuaternion(const Quaternion& q) noexcept;
		static Matrix RotationYawPitchRoll(float yaw, float pitch, float roll) noexcept;
		/**
		 * @brief  ����p�Əc����A�j�A����уt�@�[�r���[���ʋ����Ɋ�Â��������e�s����쐬����
		 * @param  fov ����p
		 * @param  aspectRatio �c����
		 * @param  nearPlane �j�A�r���[���ʋ���
		 * @param  farPlane �t�@�[�r���[���ʋ���
		 * @return �������e�s��
		 */
		static Matrix PerspectiveFieldOfView(float fov, float aspectRatio, float nearPlane, float farPlane) noexcept;
		/**
		 * @brief  
		 * @param  width 
		 * @param  height 
		 * @param  nearPlane 
		 * @param  farPlane 
		 * @return 
		 */
		static Matrix Perspective(float width, float height, float nearPlane, float farPlane) noexcept;
		static Matrix PerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane) noexcept;
		static Matrix Orthographic(float width, float height, float zNearPlane, float zFarPlane) noexcept;
		static Matrix OrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane) noexcept;
		static Matrix LookAt(const Vector3& position, const Vector3& target, const Vector3& up) noexcept;
		static Matrix World(const Vector3& position, const Vector3& forward, const Vector3& up) noexcept;
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

		/**
		 * @brief  �N�H�[�^�j�I���̎��̑O�������w���P�ʃx�N�g�������߂�
		 * @return �O�����x�N�g��
		 */
		[[nodiscard]] Vector3 Forward() const noexcept;
		/**
		 * @brief  �N�H�[�^�j�I���̎��̏�������w���P�ʃx�N�g�������߂�
		 * @return �O�����x�N�g��
		 */
		[[nodiscard]] Vector3 Up() const noexcept;
		/**
		 * @brief  �N�H�[�^�j�I���̎��̉E�������w���P�ʃx�N�g�������߂�
		 * @return �O�����x�N�g��
		 */
		[[nodiscard]] Vector3 Right() const noexcept;

		static float Dot(const Quaternion& q1, const Quaternion& q2) noexcept;
		static Quaternion Normalize(const Quaternion& q) noexcept;
		static Quaternion Conjugate(const Quaternion& q) noexcept;
		static Quaternion Inverse(const Quaternion& q) noexcept;
		static Quaternion RotationX(float rotation) noexcept;
		static Quaternion RotationY(float rotation) noexcept;
		static Quaternion RotationZ(float rotation) noexcept;
		static Quaternion AxisAngle(const Vector3& axis, float angle) noexcept;
		static Quaternion YawPitchRoll(float yaw, float pitch, float roll) noexcept;
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
		explicit Color(const DirectX::XMFLOAT4& xmf) noexcept
			: color_(DirectX::XMLoadFloat4(&xmf)) {}
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
		
		const float& operator [] (std::size_t index) const noexcept;
		float& operator [] (std::size_t index) noexcept;
		
		operator DirectX::XMVECTOR() const noexcept;
		operator DirectX::XMFLOAT4() const noexcept;

		[[nodiscard]] float& R() noexcept { return color_.m128_f32[0]; }
		[[nodiscard]] float& G() noexcept { return color_.m128_f32[1]; }
		[[nodiscard]] float& B() noexcept { return color_.m128_f32[2]; }
		[[nodiscard]] float& A() noexcept { return color_.m128_f32[3]; }

		static Color Negate(const Color& c) noexcept;
		static Color Saturate(const Color& c) noexcept;
		static Color Premultiply(const Color& c) noexcept;
		static Color AdjustSaturation(const Color& c, float sat) noexcept;
		static Color AdjustContrast(const Color& c, float contrast) noexcept;
		static Color Modulate(const Color& c1, const Color& c2) noexcept;
		static Color Lerp(const Color& c1, const Color& c2, float t) noexcept;
		static Color FromHSL(const float h, const float s, const float l, const float a = 1.0f) noexcept;
		static Color FromHSV(const float h, const float s, const float v, const float a = 1.0f) noexcept;

	private:
		DirectX::XMVECTOR color_;
	};
}

#include "core.inl"