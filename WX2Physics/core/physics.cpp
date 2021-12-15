#include "physics.h"
#include <exception>

namespace wx2::physics
{
	Physics::~Physics() noexcept
	{
		SafeRelease(physics_);

#if !defined(NDEBUG)
		if (pvd_)
		{
			pvd_->release();
		}

		if (tranceport_)
		{
			tranceport_->release();
		}
#endif

		SafeRelease(foundation_);
	}

	bool Physics::Initialize(const PxU32 numThread) noexcept
	{
		try
		{
			foundation_ = PxCreateFoundation(
				PX_PHYSICS_VERSION,allocator_,errorCallback_);
			WX2_RUNTIME_ERROR_IF_FAILED(foundation_, "PxFoundationの作成に失敗しました。");

			bool recordMemoryAllocations = true;

#if !defined(NDEBUG)
			pvd_ = PxCreatePvd(*foundation_);
			tranceport_ = PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
			pvd_->connect(*tranceport_, PxPvdInstrumentationFlag::eALL);
#endif

			scale_ = PxTolerancesScale();

			physics_ = PxCreatePhysics(
				PX_PHYSICS_VERSION,
				*foundation_,
				scale_,
				true,
				pvd_);
			WX2_RUNTIME_ERROR_IF_FAILED(physics_, "PxPhysicsの作成に失敗しました。");

			//cooking_ = PxCreateCooking(
			//	PX_PHYSICS_VERSION, *foundation_, px::PxCookingParams(scale_));
			//WX2_RUNTIME_ERROR_IF_FAILED(cooking_, "PxCoolingの作成に失敗しました。");

			const PxCudaContextManagerDesc cudaContextManagerDesc;
			cudaContextManager_ = 
				PxCreateCudaContextManager(*foundation_, cudaContextManagerDesc, PxGetProfilerCallback());
			WX2_RUNTIME_ERROR_IF_FAILED(cudaContextManager_, "PxCudaContextManagerの作成に失敗しました。");

			cpuDispacher_ = PxDefaultCpuDispatcherCreate(numThread);
			WX2_RUNTIME_ERROR_IF_FAILED(cpuDispacher_, "PxDefaultCPUDispacherの作成に失敗しました。");

			PxSceneDesc sceneDesc(scale_);
			sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
			sceneDesc.cpuDispatcher = cpuDispacher_;
			sceneDesc.filterShader = PxDefaultSimulationFilterShader;
			sceneDesc.cudaContextManager = cudaContextManager_;
			sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
			sceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;
			scene_ = physics_->createScene(sceneDesc);
			WX2_RUNTIME_ERROR_IF_FAILED(scene_, "PxSceneの作成に失敗しました。");

#if !defined(NDEBUG)
			PxPvdSceneClient* pvdClient = scene_->getScenePvdClient();
			if(!pvdClient)
			{
				WX2_LOG_WARN("PVDの設定に失敗しました。");
			}
			scene_->getScenePvdClient()->setScenePvdFlags(
				PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS | 
				PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES | 
				PxPvdSceneFlag::eTRANSMIT_CONTACTS);
#endif
		}
		catch (const RuntimeError& runtimeError)
		{
			WX2_LOG_ERROR(runtimeError.what());
			return false;
		}
		return true;
	}
}
