/*********************************************************************
 * @file   physics.h
 * @author Tomomi Murakami
 * @date   2021/11/27 19:06
 * @brief  ï®óùââéZ
 ********************************************************************/
#pragma once
#include <physx/PxPhysicsAPI.h>
#include <WX2Common.h>
#include "scene.h"
#include "rigid.h"

namespace wx2::phys
{
	class Physics final
	{
	private:

	public:
		Physics() = default;
		~Physics() noexcept;

		WX2_DISALLOW_COPY(Physics);
		WX2_DISALLOW_MOVE(Physics);

		[[nodiscard]] bool Initialize(const physx::PxU32 numThread) noexcept;

		[[nodiscard]] physx::PxMaterial* CreateMaterial(
			const float staticFriction,
			const float dynamicFriction,
			const float bounciness) const noexcept;

		[[nodiscard]] RigidDynamic CreateDynamic(
			const physx::PxTransform& transform,
			const physx::PxGeometry& geometry,
			physx::PxMaterial& material,
			physx::PxReal density = 10.0f) const noexcept;

		[[nodiscard]] RigidStatic CreateStatic(
			const physx::PxTransform& transform,
			const physx::PxGeometry& geometry,
			physx::PxMaterial& material) const noexcept;

		void Step(const float deltaTime) const noexcept;

	private:
		physx::PxFoundation* foundation_{};
		physx::PxDefaultAllocator allocator_{};
		physx::PxDefaultErrorCallback errorCallback_{};
		physx::PxPhysics* physics_{};
		physx::PxCooking* cooking_{};
		physx::PxCudaContextManager* cudaContextManager_{};
		physx::PxDefaultCpuDispatcher* cpuDispacher_{};
		Scene scene_{};
		physx::PxPvd* pvd_{};
	};
}
