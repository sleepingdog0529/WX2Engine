#include "layer_stack.h"
#include <algorithm>

namespace wx2
{
	void LayerStack::PopLayer(const LayerPtr& layer)
	{
		// レイヤースタックの通常レイヤーの範囲から対象レイヤーのイテレータを探す
		const auto itr = std::find(layers_.begin(), layers_.begin() + layerInsertIndex_, layer);
		if (itr != layers_.begin() + layerInsertIndex_)
		{
			// 対象レイヤーのポップ時のコールバックを呼び出す
			itr->get()->OnDetach();

			// レイヤースタックからポップ
			layers_.erase(itr);

			// 通常レイヤーの挿入位置をデクリメント
			--layerInsertIndex_;
		}
	}

	void LayerStack::PopOverlay(const LayerPtr& overlay)
	{
		// レイヤースタックのオーバーレイレイヤーの範囲から対象レイヤーのイテレータを探す
		const auto itr = std::find(layers_.begin() + layerInsertIndex_, layers_.end(), overlay);
		if (itr != layers_.begin() + layerInsertIndex_)
		{
			// 対象レイヤーのポップ時のコールバックを呼び出す
			itr->get()->OnDetach();

			// レイヤースタックからポップ
			layers_.erase(itr);
		}
	}
}
