/*********************************************************************
 * @file   scene_parameter.h
 * @author Tomomi Murakami
 * @date   2021/12/04 17:39
 * @brief  �V�[���֓n���p�����[�^�[
 ********************************************************************/
#pragma once
#include <any>
#include <string>
#include <unordered_map>

namespace wx2
{
	/**
	 * @brief  �V�[���Ԃŏ��`�B���邽�߂̃p�����[�^�[�N���X
	 */
	class SceneParameter final
	{
	public:
		SceneParameter() = default;
		~SceneParameter() = default;

		// �R�s�[�ƃ��[�u�֎~
		WX2_DISALLOW_COPY_AND_MOVE(SceneParameter);

		/**
		 * @brief  �p�����[�^�[�ɒl��ǉ�����
		 * @tparam T �p�����[�^�[�l�^
		 * @param  key �p�����[�^�[��
		 * @param  param �p�����[�^�[�̒l
		 */
		template <typename T>
		void Add(const std::string& key, const T& param) noexcept
		{
			parameters_[key] = param;
		}

		/**
		 * @brief  �p�����[�^�[����l���擾����
		 * @tparam T �擾����l�̌^
		 * @param  key �擾����p�����[�^�[��
		 * @return �p�����[�^�[�̒l�A���݂��Ȃ����nullopt
		 */
		template <typename T>
		std::optional<T> Get(const std::string& key) const noexcept
		{
			try
			{
				// �w�肳�ꂽ�L�[�̒l���擾����
				const auto& val = parameters_.at(key);

				// �w�肳�ꂽ�l�^�ɕϊ����ĕԂ�
				return std::any_cast<T>(val);
			}
			catch (const std::out_of_range& exception)
			{
				WX2_LOG_ERROR("�V�[���p�����[�^�[�̃L�[�����v�f�����݂��܂���");
				return std::nullopt;
			}
			catch (const std::bad_any_cast& exception)
			{
				WX2_LOG_ERROR("�V�[���p�����[�^�[���Ԉ�����^�Ŏ擾���悤�Ƃ��܂����B");
				return std::nullopt;
			}
		}

	private:
		//! �p�����[�^�̃}�b�v
		std::unordered_map<std::string, std::any> parameters_{};
	};
}
