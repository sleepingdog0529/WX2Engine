/*********************************************************************
 * @file   model_loader.h
 * @author Tomomi Murakami
 * @date   2021/11/29 16:07
 * @brief  ÉÇÉfÉãì«çû
 ********************************************************************/
#pragma once
#include <memory>
#include <filesystem>
#include "model.h"

namespace wx2
{
	class ModelLoader final
	{
		using ModelPtr = std::shared_ptr<Model>;

	public:
		static void Initialize(Device* devices);

		static void LoadAsync(const std::string& key, const std::filesystem::path& filePath);
		static ModelPtr Get(const std::string& key);

	private:
		static inline Device* devices_{};
		static inline std::unordered_map<std::string, std::future<ModelPtr>, StringHash> loadingModels_{};
		static inline std::unordered_map<std::string, ModelPtr, StringHash> models_{};
		static inline ThreadPool threadPool_{8};
		static inline std::mutex mutex_{};
	};
}
