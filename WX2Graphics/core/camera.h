/*********************************************************************
 * @file   camera.h
 * @author Tomomi Murakami
 * @date   2021/12/19 20:20
 * @brief  ÉJÉÅÉâ
 ********************************************************************/
#pragma once
#include <WX2Mathematics.h>

namespace wx2
{
	class Camera : public Transform
	{
	public:
		Camera() = default;
		virtual ~Camera() override = default;

		WX2_COPYABLE(Camera);
		WX2_MOVEABLE(Camera);

		[[nodiscard]] const Matrix& GetProjectionMatrix() const noexcept { return projection_; }

		void SetProjectionValues(
			float fov,
			float aspectRatio, 
			float nearPlane,
			float farPlane) noexcept;

	private:
		Matrix projection_{};
	};
}
