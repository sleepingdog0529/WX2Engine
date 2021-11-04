#include "model.h"
#include "util/file_explorer.h"

namespace wx2 {

	static const auto kTextureDirectory = ".\\model";

	Model::Model(Graphics* graphics, std::vector<Mesh>& meshes) :
		graphics_(graphics),
		meshes_(std::move(meshes))
	{
	}

	void Model::Draw(const XMMATRIX& world)
	{
		for (const auto& m : meshes_)
		{
			m.Draw(world);
		}
	}

}