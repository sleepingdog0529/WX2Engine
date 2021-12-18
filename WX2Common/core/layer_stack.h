/*********************************************************************
 * @file   layer_stack.h
 * @author Tomomi Murakami
 * @date   2021/12/14 15:07
 * @brief  レイヤースタック
 ********************************************************************/
#pragma once
#include "layer.h"
#include <vector>
#include <memory>

namespace wx2
{
	/**
	 * @brief レイヤースタッククラス
	 */
	class LayerStack final
	{
		//! レイヤーの共有ポインタ
		using LayerPtr = std::shared_ptr<ILayer>;

	public:
		LayerStack() = default;
		~LayerStack() = default;

		WX2_DISALLOW_COPY(LayerStack);
		WX2_DISALLOW_MOVE(LayerStack);

		/**
		 * @brief  レイヤーをスタック内に直接構築する
		 * @tparam LayerType レイヤー型
		 * @tparam Args コンストラクタ引数型
		 * @param  layerName レイヤーの名前
		 * @param  args コンストラクタ引数
		 * @return 構築されたレイヤーの共有ポインタ
		 */
		template <class LayerType, typename... Args>
		std::shared_ptr<LayerType> EmplaceLayer(const std::string& layerName, Args&&... args)
		{
			// 通常レイヤーの挿入位置にレイヤーを直接構築
			const auto itr = layers_.emplace(
				layers_.begin() + layerInsertIndex_,
				std::make_shared<LayerType>(layerName, std::forward<Args>(args)...));

			// 通常レイヤーの挿入位置をインクリメント
			++layerInsertIndex_;

			// 構築したレイヤーの共有ポインタを返す
			return *itr;
		}

		/**
		 * @brief  オーバーレイレイヤーをスタック内に直接構築する
		 * @tparam OverlayType レイヤー型
		 * @tparam Args コンストラクタ引数型
		 * @param  layerName レイヤーの名前
		 * @param  args コンストラクタ引数
		 * @return 構築されたレイヤーの共有ポインタ
		 */
		template <class OverlayType, typename... Args>
		std::shared_ptr<OverlayType> EmplaceOverlay(const std::string& layerName, Args&&... args)
		{
			// レイヤーリスト末尾にレイヤーを直接構築
			const auto itr = layers_.emplace_back(
				std::make_shared<OverlayType>(layerName, std::forward<Args>(args)...));

			// 構築したレイヤーの共有ポインタを返す
			return *itr;
		}

		/**
		 * @brief  レイヤーをレイヤースタックからポップする
		 * @param  layer 解放するレイヤーの共有ポインタ
		 */
		void PopLayer(const LayerPtr& layer);

		/**
		 * @brief  オーバーレイレイヤーをレイヤースタックからポップする
		 * @param  overlay 解放するレイヤーの共有ポインタ
		 */
		void PopOverlay(const LayerPtr& overlay);

	private:
		//! レイヤースタック
		std::vector<LayerPtr> layers_{};
		//! 通常レイヤーの挿入位置
		std::size_t layerInsertIndex_{};
	};	
}
