/*********************************************************************
 * @file   layer.h
 * @author Tomomi Murakami
 * @date   2021/12/14 14:20
 * @brief  レイヤーインターフェイス
 ********************************************************************/
#pragma once
#include <string>
#include "../util/macro.h"

namespace wx2
{
	/**
	 * @brief レイヤーのインターフェイスクラス
	 */
	class ILayer
	{
	public:
		/**
		 * @brief  レイヤー情報の初期化
		 * @param  layerName レイヤーの名前
		 */
		explicit ILayer(std::string layerName)
			: layerName_(std::move(layerName)) {}
		virtual ~ILayer() = default;

		// コピーとムーブの禁止
		WX2_DISALLOW_COPY_AND_MOVE(ILayer);

		//! @brief レイヤースタックにプッシュされたときのコールバック
		virtual void OnAttach() {}
		//! @brief レイヤースタックからポップされたときのコールバック
		virtual void OnDetach() {}

		/**
		 * @brief  レイヤー名の取得
		 * @return レイヤー名文字列
		 */
		[[nodiscard]] const std::string& GetName() const noexcept { return layerName_; }

	private:
		//! レイヤー名
		std::string layerName_;
	};
}
