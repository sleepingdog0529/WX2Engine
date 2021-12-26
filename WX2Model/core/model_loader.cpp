#include "model_loader.h"
#include <boost/lexical_cast.hpp>

namespace wx2
{
	void ModelLoader::Initialize(Device* devices)
	{
		WX2_ASSERT_MSG(devices, "デバイスがnullptrでした。");

		devices_ = devices;
	}

	void ModelLoader::LoadAsync(const std::string& key, const std::filesystem::path& filePath)
	{
		auto future = threadPool_.Enqueue(
			[filePath]()
			{
				return std::make_shared<Model>(devices_, filePath);
			});

		loadingModels_.insert_or_assign(key, std::move(future));
	}

	ModelLoader::ModelPtr ModelLoader::Get(const std::string& key)
	{
		if (const auto itr = models_.find(key); itr != models_.end())
		{
			return itr->second;
		}

		if (const auto itr = loadingModels_.find(key); itr != loadingModels_.end())
		{
			const auto res = itr->second.wait_for(std::chrono::nanoseconds::zero());
			if (res != std::future_status::timeout)
			{
				const auto model = itr->second.get();

				{
					std::lock_guard lock(mutex_);

					models_.insert_or_assign(key, model);
					loadingModels_.erase(itr);
				}

				return model;
			}
		}

		return nullptr;
	}
}