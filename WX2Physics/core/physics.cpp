#include "physics.h"
#include <exception>

namespace wx2
{
	Physics::~Physics() noexcept
	{
		SafeRelease(physics_);
		
		if (pvd_)
		{
			pvd_->release();
		}

		SafeRelease(foundation_);
	}

	bool Physics::Initialize(const PxU32 numThread) noexcept
	{
		try
		{
			foundation_ = PxCreateFoundation(
				PX_PHYSICS_VERSION,allocator_,errorCallback_);
			WX2_RUNTIME_ERROR_IF_FAILED(foundation_, "PxFoundation‚Ìì¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");

			bool recordMemoryAllocations = true;
			
			pvd_ = PxCreatePvd(*foundation_);
			PxPvdTransport* tranceport = PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
			pvd_->connect(*tranceport, PxPvdInstrumentationFlag::eALL);

			physics_ = PxCreatePhysics(
				PX_PHYSICS_VERSION,
				*foundation_,
				PxTolerancesScale(),
				true,
				pvd_);
			WX2_RUNTIME_ERROR_IF_FAILED(physics_, "PxPhysics‚Ìì¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");

			cooking_ = PxCreateCooking(
				PX_PHYSICS_VERSION, *foundation_, PxCookingParams(scale_));
			WX2_RUNTIME_ERROR_IF_FAILED(cooking_, "PxCooling‚Ìì¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");

			cpuDispacher_ = PxDefaultCpuDispatcherCreate(numThread);
			WX2_RUNTIME_ERROR_IF_FAILED(cpuDispacher_, "PxDefaultCPUDispacher‚Ìì¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");

			PxSceneDesc sceneDesc(scale_);
			sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
			sceneDesc.cpuDispatcher = cpuDispacher_;
			sceneDesc.filterShader = PxDefaultSimulationFilterShader;
			sceneDesc.cudaContextManager = cudaContextManager_;
			sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
			sceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;
			scene_ = physics_->createScene(sceneDesc);
			WX2_RUNTIME_ERROR_IF_FAILED(scene_, "PxScene‚Ìì¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");
			
			PxPvdSceneClient* pvdClient = scene_->getScenePvdClient();
			WX2_RUNTIME_ERROR_IF_FAILED(pvdClient, "PVD‚ÌÝ’è‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");

			scene_->getScenePvdClient()->setScenePvdFlags(
				PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS | 
				PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES | 
				PxPvdSceneFlag::eTRANSMIT_CONTACTS);

			return true;
		}
		catch (const RuntimeError& runtimeError)
		{
			WX2_LOG_ERROR(runtimeError.what());
			return false;
		}
	}

	PxMaterial* Physics::CreateMaterial(
		const float staticFriction,
		const float dynamicFriction,
		const float bounciness) const noexcept
	{
		return physics_->createMaterial(staticFriction, dynamicFriction, bounciness);
	}

	PxRigidDynamic* Physics::CreateDynamic(
		const PxTransform& transform, 
		const PxGeometry& geometry, 
		PxMaterial& material,
		const PxReal density) const noexcept
	{
		PxRigidDynamic* rigidDynamic = 
			PxCreateDynamic(*physics_, transform, geometry, material, density);
		scene_->addActor(*rigidDynamic);
		return rigidDynamic;
	}

	void Physics::Step(const float deltaTime) const noexcept
	{
		scene_->simulate(deltaTime);
		scene_->fetchResults(true);
	}
}
