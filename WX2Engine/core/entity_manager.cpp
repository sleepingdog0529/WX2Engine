#include "entity_manager.h"

namespace wx2
{
	EntityManager::EntityManager() noexcept
	{
		// ���p�\�ȃG���e�B�e�BID��������
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			availableEntities_.push(entity);
		}
	}

	Entity EntityManager::CreateEntity()
	{
		WX2_RUNTIME_ERROR_IF_FAILED(
			livingEntityCount_ < MAX_ENTITIES, "�G���e�B�e�B���������܂��B");

		// ���g�p�G���e�B�e�BID���|�b�v���Ďg�p
		const Entity id = availableEntities_.front();
		availableEntities_.pop();
		++livingEntityCount_;

		return id;
	}

	void EntityManager::DestroyEntity(const Entity entity)
	{
		WX2_ASSERT_MSG(entity < MAX_ENTITIES, "�G���e�B�e�BID���͈͊O�ł��B");

		// �G���e�B�e�B��������ĕK�v�Ȃ��Ȃ����̂ŃV�O�l�`�������Z�b�g
		signatures_[entity].reset();

		// ���g�p�G���e�B�e�BID�ɉ�����
		availableEntities_.push(entity);
		--livingEntityCount_;
	}

	void EntityManager::SetSignature(const Entity entity, const Signature& signature)
	{
		WX2_ASSERT_MSG(entity < MAX_ENTITIES, "�G���e�B�e�BID���͈͊O�ł��B");

		// �G���e�B�e�BID���w���V�O�l�`�����㏑��
		signatures_[entity] = signature;
	}

	EntityManager::Signature EntityManager::GetSignature(const Entity entity) const noexcept
	{
		WX2_ASSERT_MSG(entity < MAX_ENTITIES, "�G���e�B�e�BID���͈͊O�ł��B");

		// �G���e�B�e�BID���w���V�O�l�`����Ԃ�
		return signatures_[entity];
	}
}