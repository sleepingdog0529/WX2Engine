#pragma once

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

	inline Vector2 Vector2::operator*(float rhs) const noexcept
	{
		return Vector2(DirectX::XMVectorScale(vector_, rhs));
	}

	inline Vector2 Vector2::operator/(float rhs) const noexcept
	{
		WX2_ASSERT_MSG(rhs != 0.0f, "0èúéZÇ™î≠ê∂ÇµÇ‹ÇµÇΩÅB");
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

	inline Vector2& Vector2::operator*=(float rhs) noexcept
	{
		vector_ = DirectX::XMVectorScale(vector_, rhs);
		return *this;
	}

	inline Vector2& Vector2::operator/=(float rhs) noexcept
	{
		WX2_ASSERT_MSG(rhs != 0.0f, "0èúéZÇ™î≠ê∂ÇµÇ‹ÇµÇΩÅB");
		vector_ = DirectX::XMVectorScale(vector_, 1.0f / rhs);
		return *this;
	}

	inline const float& Vector2::operator[](std::size_t index) const noexcept
	{
		WX2_ASSERT_MSG(index < 2, "ìYÇ¶éöÇÃílÇ™îÕàÕäOÇ≈Ç∑ÅB");
		return vector_.m128_f32[index];
	}

	inline float& Vector2::operator[](std::size_t index) noexcept
	{
		WX2_ASSERT_MSG(index < 2, "ìYÇ¶éöÇÃílÇ™îÕàÕäOÇ≈Ç∑ÅB");
		return vector_.m128_f32[index];
	}

	inline Vector2::operator DirectX::XMVECTOR() const noexcept
	{
		return vector_;
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

	inline Vector2 Vector2::Normalize(const Vector2& v) noexcept
	{
		return Vector2(DirectX::XMVector2Normalize(v.vector_));
	}

	inline float Vector2::Dot(const Vector2& v1, const Vector2& v2) noexcept
	{
		return DirectX::XMVectorGetX(DirectX::XMVector2Dot(v1.vector_, v2.vector_));
	}

	inline Vector2 Vector2::Cross(const Vector2& v1, const Vector2& v2) noexcept
	{
		return Vector2(DirectX::XMVector2Cross(v1.vector_, v2.vector_));
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
		WX2_ASSERT_MSG(rhs != 0.0f, "0èúéZÇ™î≠ê∂ÇµÇ‹ÇµÇΩÅB");
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
		WX2_ASSERT_MSG(rhs != 0.0f, "0èúéZÇ™î≠ê∂ÇµÇ‹ÇµÇΩÅB");
		vector_ = DirectX::XMVectorScale(vector_, 1.0f / rhs);
		return *this;
	}

	inline const float& Vector3::operator[](const std::size_t index) const noexcept
	{
		WX2_ASSERT_MSG(index < 3, "ìYÇ¶éöÇÃílÇ™îÕàÕäOÇ≈Ç∑ÅB");
		return vector_.m128_f32[index];
	}

	inline float& Vector3::operator[](const std::size_t index) noexcept
	{
		WX2_ASSERT_MSG(index < 3, "ìYÇ¶éöÇÃílÇ™îÕàÕäOÇ≈Ç∑ÅB");
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

	inline Vector3 Vector3::Normalize(const Vector3& v) noexcept
	{
		return Vector3(DirectX::XMVector3Normalize(v));
	}

	inline float Vector3::Dot(const Vector3& v1, const Vector3& v2) noexcept
	{
		return DirectX::XMVectorGetX(DirectX::XMVector3Dot(v1, v2));
	}

	inline Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2) noexcept
	{
		return Vector3(DirectX::XMVector3Cross(v1, v2));
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
		WX2_ASSERT_MSG(rhs != 0.0f, "0èúéZÇ™î≠ê∂ÇµÇ‹ÇµÇΩÅB");
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
		WX2_ASSERT_MSG(rhs != 0.0f, "0èúéZÇ™î≠ê∂ÇµÇ‹ÇµÇΩÅB");
		vector_ = DirectX::XMVectorScale(vector_, 1.0f / rhs);
		return *this;
	}

	inline const float& Vector4::operator[](const std::size_t index) const noexcept
	{
		WX2_ASSERT_MSG(index < 4, "ìYÇ¶éöÇÃílÇ™îÕàÕäOÇ≈Ç∑ÅB");
		return vector_.m128_f32[index];
	}

	inline float& Vector4::operator[](const std::size_t index) noexcept
	{
		WX2_ASSERT_MSG(index < 4, "ìYÇ¶éöÇÃílÇ™îÕàÕäOÇ≈Ç∑ÅB");
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

	inline Vector4 Vector4::Normalize(const Vector4& v) noexcept
	{
		return Vector4(DirectX::XMVector4Normalize(v.vector_));
	}

	inline float Vector4::Dot(const Vector4& v1, const Vector4& v2) noexcept
	{
		return DirectX::XMVectorGetX(DirectX::XMVector4Dot(v1.vector_, v2.vector_));
	}

	inline Vector4 Vector4::Cross(const Vector4& v1, const Vector4& v2, const Vector4& v3) noexcept
	{
		return Vector4(DirectX::XMVector4Cross(v1.vector_, v2.vector_, v3.vector_));
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
		WX2_ASSERT_MSG(index < 4, "ìYÇ¶éöÇÃílÇ™îÕàÕäOÇ≈Ç∑ÅB");
		return Vector4(matrix_.r[index]);
	}

	inline Vector4& Matrix::operator[](const std::size_t index) noexcept
	{
		WX2_ASSERT_MSG(index < 4, "ìYÇ¶éöÇÃílÇ™îÕàÕäOÇ≈Ç∑ÅB");
		return reinterpret_cast<Vector4&>(matrix_.r[index]);
	}

	inline Matrix::operator DirectX::XMMATRIX() const noexcept
	{
		return matrix_;
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

	inline Matrix Matrix::RotationX(const float radians) noexcept
	{
		return Matrix(DirectX::XMMatrixRotationX(radians));
	}

	inline Matrix Matrix::RotationY(const float radians) noexcept
	{
		return Matrix(DirectX::XMMatrixRotationY(radians));
	}

	inline Matrix Matrix::RotationZ(const float radians) noexcept
	{
		return Matrix(DirectX::XMMatrixRotationZ(radians));
	}

	inline Matrix Matrix::FromAxisAngle(const Vector3& axis, const float angle) noexcept
	{
		return Matrix(DirectX::XMMatrixRotationAxis(axis, angle));
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
		return Matrix(DirectX::XMMatrixLookAtRH(position, target, up));
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

	inline Matrix Matrix::FromYawPitchRoll(const float yaw, const float pitch, const float roll) noexcept
	{
		return Matrix(DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
	}

	inline Matrix Matrix::Lerp(const Matrix& m1, const Matrix& m2, float t) noexcept
	{
		return {
			Vector4(DirectX::XMVectorLerp(m1[0], m2[0], t)),
			Vector4(DirectX::XMVectorLerp(m1[1], m2[1], t)),
			Vector4(DirectX::XMVectorLerp(m1[2], m2[2], t)),
			Vector4(DirectX::XMVectorLerp(m1[3], m2[3], t)) };
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

	inline Quaternion& Quaternion::operator *= (float rhs) noexcept
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

	inline float Quaternion::Dot(const Quaternion& q1, const Quaternion& q2) noexcept
	{
		return DirectX::XMVectorGetX(DirectX::XMQuaternionDot(q1.quaternion_, q2.quaternion_));
	}

	inline Quaternion Quaternion::FromAxisAngle(const Vector3& axis, const float angle) noexcept
	{
		return Quaternion(DirectX::XMQuaternionRotationAxis(axis, angle));
	}

	inline Quaternion Quaternion::FromYawPitchRoll(const float yaw, const float pitch, const float roll) noexcept
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

#pragma endregion Quaternion
}