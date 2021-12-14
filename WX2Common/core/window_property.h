/*********************************************************************
 * @file	window_property.h
 * @author	Tomomi Murakami
 * @date	2021/09/20 19:19
 * @brief	ウィンドウ情報
 ********************************************************************/
#pragma once
#pragma warning(push, 0)
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#pragma warning(pop)

namespace wx2 
{
	/**
	 * @brief ウィンドウ情報
	 */
	struct WindowProperty
	{
	public:
		std::string title;		//! ウィンドウのタイトル
		int x, y;				//! ウィンドウ左上の座標
		int width, height;		//! ウィンドウの幅
		unsigned long style;	//! ウィンドウスタイルのフラグ
		unsigned long exStyle;	//! 拡張ウィンドウスタイルのフラグ
		bool maximized;			//! 最大化されているか
		bool fullscreen;		//! 全画面化されているか

		/**
		 * @brief  ウィンドウのアスペクト比を返す
		 * @return アスペクト比
		 */
		[[nodiscard]] float AspectRatio() const noexcept
		{
			return static_cast<float>(width) / static_cast<float>(height);
		}

	private:
		// シリアル化ライブラリのアクセスを許可
		friend cereal::access;

		//! @brief シリアライズ用メソッド
		template <class Archive>
		void serialize(Archive& archive) noexcept
		{
			archive(
				CEREAL_NVP(title),
				CEREAL_NVP(x),
				CEREAL_NVP(y),
				CEREAL_NVP(width),
				CEREAL_NVP(height),
				CEREAL_NVP(style),
				CEREAL_NVP(exStyle),
				CEREAL_NVP(maximized),
				CEREAL_NVP(fullscreen));
		}
	};
}