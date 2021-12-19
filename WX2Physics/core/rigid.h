/*********************************************************************
 * @file   rigid.h
 * @author Tomomi Murakami
 * @date   2021/12/18 18:25
 * @brief  çÑëÃ
 ********************************************************************/
#pragma once
#include <memory>
#include <physx/PxRigidActor.h>
#include <WX2Mathematics.h>
#include "scene.h"

namespace wx2::phys
{
	class RigidActor
	{
	public:
		RigidActor() = default;
		virtual ~RigidActor() = default;

		WX2_COPYABLE(RigidActor);
		WX2_MOVEABLE(RigidActor);

		void SetPosition(const Vector3& position) const noexcept;
		void SetRotation(const Quaternion& rotation) const noexcept;

		[[nodiscard]] Vector3 GetPosition() const noexcept;
		[[nodiscard]] Quaternion GetRotation() const noexcept;

	protected:
		physx::PxRigidActor* rigidActor_;
	};

	class RigidDynamic : public RigidActor
	{
	public:
		RigidDynamic() = default;
		virtual ~RigidDynamic() override = default;

		void Initialize(
			physx::PxPhysics& physics,
			const Scene& scene,
			const physx::PxTransform& transform,
			const physx::PxGeometry& geometry,
			physx::PxMaterial& material,
			physx::PxReal density = 10.0f) noexcept;

		WX2_COPYABLE(RigidDynamic);
		WX2_MOVEABLE(RigidDynamic);
	};

	class RigidStatic : public RigidActor
	{
	public:
		RigidStatic() = default;
		virtual ~RigidStatic() override = default;

		void Initialize(
			physx::PxPhysics& physics,
			const Scene& scene,
			const physx::PxTransform& transform,
			const physx::PxGeometry& geometry,
			physx::PxMaterial& material) noexcept;

		WX2_COPYABLE(RigidStatic);
		WX2_MOVEABLE(RigidStatic);
	};
}
