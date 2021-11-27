/*********************************************************************
 * @file   physics.h
 * @author Tomomi Murakami
 * @date   2021/11/27 19:06
 * @brief  �������Z
 ********************************************************************/
#pragma once
#include <physx/PxPhysicsAPI.h>
#include <memory>
#include <WX2Common.h>

namespace wx2::physics
{
	using namespace physx; // Px���ăv���t�B�b�N�X����Ȃ�using���ėǂ����H

	class Physics final
	{
	private:

	public:
		Physics() = default;
		~Physics() noexcept;

		WX2_DISALLOW_COPY_AND_MOVE(Physics);

		[[nodiscard]] bool Initialize(const PxU32 numThread) noexcept;

	private:
		PxFoundation* foundation_{};
		PxDefaultAllocator allocator_{};
		PxDefaultErrorCallback errorCallback_{};
		PxTolerancesScale scale_{};
		PxPhysics* physics_{};
		//PxCooking* cooking_{};
		PxCudaContextManager* cudaContextManager_{};
		PxDefaultCpuDispatcher* cpuDispacher_{};
		PxScene* scene_{};

#if !defined(NDEBUG)
		physx::PxPvd* pvd_{};
		PxPvdTransport* tranceport_{};
#endif
	};
}
