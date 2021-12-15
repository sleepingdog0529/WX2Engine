/*********************************************************************
 * @file   scene_parameter.h
 * @author Tomomi Murakami
 * @date   2021/12/04 17:39
 * @brief  �V�[���֓n���p�����[�^�[
 ********************************************************************/
#pragma once
#include <variant>
#include <string>
#include <unordered_map>

namespace wx2
{
	/**
	 * @brief  �V�[���Ԃŏ��`�B���邽�߂̃p�����[�^�[�N���X
	 */
	class SceneParameter final
	{
	private:
		// �p�����[�^�[�Ƃ��Đݒ�ł���l�^
		using ParameterType = std::variant<int, std::string>;

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
			// �w�肳�ꂽ�L�[������Ȃ����A�l�̌^���������nullopt��Ԃ�
			const auto itr = parameters_.find(key);
			if (itr == parameters_.end() || 
				!std::holds_alternative<T>(itr->second))
			{
				return std::nullopt;
			}

			// �w�肳�ꂽ�l�^�ɕϊ����ĕԂ�
			return std::get<T>(itr->second);
		}

	private:
		//! �p�����[�^�̃}�b�v
		std::unordered_map<std::string, ParameterType> parameters_{};
	};
}
