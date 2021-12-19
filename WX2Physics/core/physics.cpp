#include "physics.h"
#include <exception>

namespace wx2::phys
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

	bool Physics::Initialize(const physx::PxU32 numThread) noexcept
	{
		try
		{
			foundation_ = PxCreateFoundation(
				PX_PHYSICS_VERSION,allocator_,errorCallback_);
			WX2_RUNTIME_ERROR_IF_FAILED(foundation_, "PxFoundation‚Ìì¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");

			pvd_ = PxCreatePvd(*foundation_);
			physx::PxPvdTransport* tranceport = physx::PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
			pvd_->connect(*tranceport, physx::PxPvdInstrumentationFlag::eALL);

			physics_ = PxCreatePhysics(
				PX_PHYSICS_VERSION,
				*foundation_,
				physx::PxTolerancesScale(),
				true,
				pvd_);
			WX2_RUNTIME_ERROR_IF_FAILED(physics_, "PxPhysics‚Ìì¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");

			cooking_ = PxCreateCooking(
				PX_PHYSICS_VERSION, *foundation_, physx::PxCookingParams(physics_->getTolerancesScale()));
			WX2_RUNTIME_ERROR_IF_FAILED(cooking_, "PxCooling‚Ìì¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");

			cpuDispacher_ = physx::PxDefaultCpuDispatcherCreate(numThread);
			WX2_RUNTIME_ERROR_IF_FAILED(cpuDispacher_, "PxDefaultCPUDispacher‚Ìì¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");

			//physx::PxSceneDesc sceneDesc(physics_->getTolerancesScale());
			//sceneDesc.gravity = physx::PxVec3(0.0f, -9.8f, 0.0f);
			//sceneDesc.cpuDispatcher = cpuDispacher_;
			//sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
			//sceneDesc.cudaContextManager = cudaContextManager_;
			//sceneDesc.flags |= physx::PxSceneFlag::eENABLE_GPU_DYNAMICS;
			//sceneDesc.broadPhaseType = physx::PxBroadPhaseType::eGPU;
			//scene_ = physics_->createScene(sceneDesc);
			//WX2_RUNTIME_ERROR_IF_FAILED(scene_, "PxScene‚Ìì¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");
			
			bool res = scene_.Initialize(*physics_, *cpuDispacher_, *cudaContextManager_);
			WX2_RUNTIME_ERROR_IF_FAILED(res, "•¨—‰‰ŽZƒV[ƒ“‚Ìì¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");

			physx::PxPvdSceneClient* pvdClient = scene_->getScenePvdClient();
			if (pvdClient)
			{
				scene_->getScenePvdClient()->setScenePvdFlags(
					physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS |
					physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES |
					physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS);
			}
			else
			{
				WX2_LOG_INFO("PVDƒNƒ‰ƒCƒAƒ“ƒg‚ª‹N“®‚³‚ê‚Ä‚¢‚Ü‚¹‚ñB");
			}

			return true;
		}
		catch (const RuntimeError& runtimeError)
		{
			WX2_LOG_ERROR(runtimeError.what());
			return false;
		}
	}

	physx::PxMaterial* Physics::CreateMaterial(
		const float staticFriction,
		const float dynamicFriction,
		const float bounciness) const noexcept
	{
		return physics_->createMaterial(staticFriction, dynamicFriction, bounciness);
	}

	RigidDynamic Physics::CreateDynamic(
		const physx::PxTransform& transform, 
		const physx::PxGeometry& geometry,
		physx::PxMaterial& material,
		const physx::PxReal density) const noexcept
	{
		RigidDynamic rd;
		rd.Initialize(*physics_, scene_, transform, geometry, material, density);
		return rd;
	}

	RigidStatic Physics::CreateStatic(
		const physx::PxTransform& transform,
		const physx::PxGeometry& geometry, 
		physx::PxMaterial& material) const noexcept
	{
		RigidStatic rs;
		rs.Initialize(*physics_, scene_, transform, geometry, material);
		return rs;
	}

	void Physics::Step(const float deltaTime) const noexcept
	{
		scene_->simulate(deltaTime);
		scene_->fetchResults(true);
	}
}
