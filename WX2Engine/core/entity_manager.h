/*********************************************************************
 * @file   entity_manager.h
 * @author Tomomi Murakami
 * @date   2021/12/04 18:06
 * @brief  ECS�̃G���e�B�e�B�}�l�[�W���[
 ********************************************************************/
#pragma once
#include "entity.h"

namespace wx2
{

	/**
	 * @brief  �G���e�B�e�BID�ƃV�O�l�`�����Ǘ�����
	 */
	class EntityManager
	{
	private:
		//! �R���|�[�l���g�̍ő吔
		static constexpr std::size_t MAX_COMPONENTS = 32;

		//! �G���e�B�e�B�����R���|�[�l���gID���r�b�g��ŕێ�����
		using Signature = std::bitset<MAX_COMPONENTS>;

	public:
		EntityManager() noexcept;
		virtual ~EntityManager() = default;

		WX2_DISALLOW_COPY_AND_MOVE(EntityManager);

		/**
		 * @brief  �G���e�B�e�B�𐶐�����
		 * @return ���������G���e�B�e�BID
		 */
		Entity CreateEntity();

		/**
		 * @brief  �G���e�B�e�B���폜����
		 * @param  entity �폜����G���e�B�e�BID
		 */
		void DestroyEntity(const Entity entity);

		/**
		 * @brief  �G���e�B�e�B�ɃV�O�l�`�����Z�b�g����
		 * @param  entity �Z�b�g��̃G���e�B�e�BID
		 * @param  signature �G���e�B�e�B�ɃZ�b�g����V�O�l�`��
		 */
		void SetSignature(const Entity entity, const Signature& signature);

		/**
		 * @brief  �G���e�B�e�B�̃V�O�l�`�����擾����
		 * @param  entity �擾��̃G���e�B�e�BID
		 * @return �G���e�B�e�B�̃V�O�l�`��
		 */
		[[nodiscard]] Signature GetSignature(const Entity entity) const noexcept;

	private:
		//! �g�p���Ă��Ȃ��G���e�B�e�BID�̃��X�g
		std::queue<Entity> availableEntities_{};

		//! �G���e�B�e�B�̃V�O�l�`���z��
		std::array<Signature, MAX_ENTITIES> signatures_{};

		//! �g�p����Ă���G���e�B�e�B�̐�
		uint32_t livingEntityCount_{};
	};
}
