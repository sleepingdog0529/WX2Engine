#include "rigid.h"

#include <physx/PxRigidDynamic.h>
#include <physx/PxRigidStatic.h>
#include <physx/extensions/PxSimpleFactory.h>

namespace wx2::phys
{
	void RigidActor::SetPosition(const Vector3& position) const noexcept
	{
		auto transform = rigidActor_->getGlobalPose();
		transform.p = { position[0], position[1], position[2] };
		rigidActor_->setGlobalPose(transform);
	}

	void RigidActor::SetRotation(const Quaternion& rotation) const noexcept
	{
		auto transform = rigidActor_->getGlobalPose();
		transform.q = { rotation[0], rotation[1], rotation[2], rotation[3] };
		rigidActor_->setGlobalPose(transform);
	}

	Vector3 RigidActor::GetPosition() const noexcept
	{
		const auto transform = rigidActor_->getGlobalPose();
		return { transform.p.x, transform.p.y, transform.p.z };
	}

	Quaternion RigidActor::GetRotation() const noexcept
	{
		const auto transform = rigidActor_->getGlobalPose();
		return { transform.q.x, transform.q.y, transform.q.z, transform.q.w };
	}

	void RigidDynamic::Initialize(
		physx::PxPhysics& physics,
		const Scene& scene,
		const physx::PxTransform& transform,
		const physx::PxGeometry& geometry, 
		physx::PxMaterial& material,
		const physx::PxReal density) noexcept
	{
		rigidActor_ = PxCreateDynamic(physics, transform, geometry, material, density);
		scene->addActor(*rigidActor_);
	}

	void RigidStatic::Initialize(physx::PxPhysics& physics, const Scene& scene, const physx::PxTransform& transform, const physx::PxGeometry& geometry, physx::PxMaterial& material) noexcept
	{
		rigidActor_ = PxCreateStatic(physics, transform, geometry, material);
		scene->addActor(*rigidActor_);
	}
}
