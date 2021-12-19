#include "transform.h"

namespace wx2
{
	void Transform::SetLookAt(const Vector3& target, const Vector3& up) noexcept
	{
		rotation_ = Quaternion::LookAtRotation(position_, target, up);
	}

	void Transform::SetLookTo(const Vector3& forward, const Vector3& up) noexcept
	{
		rotation_ = Quaternion::LookToRotation(position_, forward, up);
	}

	Matrix Transform::GetWorldMatrix() const noexcept
	{
		return
			Matrix::Scale(scale_) *
			Matrix::RotationFromQuaternion(rotation_),
			Matrix::Translation(position_);
	}

	Matrix Transform::GetViewMatrix() const noexcept
	{
		return Matrix::LookTo(position_, rotation_.Forward());
	}
}
