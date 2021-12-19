#include "scene.h"
#include <physx/PxPhysicsAPI.h>
#include <WX2Common.h>

namespace wx2::phys
{
	bool Scene::Initialize(
		physx::PxPhysics& physics,
		physx::PxCpuDispatcher& cpuDispatcher,
		physx::PxCudaContextManager& cudaContextManager) noexcept
	{
		physx::PxSceneDesc sceneDesc(physics.getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.8f, 0.0f);
		sceneDesc.cpuDispatcher = &cpuDispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		sceneDesc.cudaContextManager = &cudaContextManager;
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_GPU_DYNAMICS;
		sceneDesc.broadPhaseType = physx::PxBroadPhaseType::eGPU;
		scene_ = physics.createScene(sceneDesc);
		WX2_RUNTIME_ERROR_IF_FAILED(scene_, "PxScene‚Ìì¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");
	}

	physx::PxScene* Scene::operator-> () const noexcept
	{
		return scene_;
	}
}
