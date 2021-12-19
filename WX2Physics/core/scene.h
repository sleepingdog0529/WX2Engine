/*********************************************************************
 * @file   scene.h
 * @author Tomomi Murakami
 * @date   2021/12/18 19:05
 * @brief  •¨—‰‰ZƒV[ƒ“
 ********************************************************************/
#pragma once
#include <memory>
#include <physx/PxScene.h>
#include <WX2Common.h>

namespace wx2::phys
{
	class Scene final
	{
	public:
		Scene() = default;
		~Scene() = default;

		[[nodiscard]] bool Initialize(
			physx::PxPhysics& physics,
			physx::PxCpuDispatcher& cpuDispatcher, 
			physx::PxCudaContextManager& cudaContextManager) noexcept;

		physx::PxScene* operator-> () const noexcept;

	private:
		physx::PxScene* scene_;
	};
}
