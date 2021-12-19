#include "camera.h"

namespace wx2
{
	void Camera::SetProjectionValues(
		const float fov, 
		const float aspectRatio, 
		const float nearPlane,
		const float farPlane) noexcept
	{
		projection_ = 
			Matrix::PerspectiveFieldOfView(fov, aspectRatio, nearPlane, farPlane);
	}
}
