#include "util/Globals.hpp"
#include "util/AssetManager.hpp"
#include "render/DDSImage.hpp"

#include "AssetUtils.hpp"

using namespace util;
using namespace render;

#define GET_ASSET(id) AssetManager::getAssetManager()->getAsset(id)

Material *AssetUtils::getMaterial(int assetId, int materialId)
{
	Asset *asset = GET_ASSET(assetId);
	if(asset==0)
	{
		util::Globals::fatalError("No such material library (MTL) "+std::to_string(assetId));
		return 0;
	}
	MaterialLibrary *mtl = dynamic_cast<MaterialLibrary *>(asset);
	if(mtl==0)
	{
		util::Globals::fatalError("No such material "+std::to_string(materialId)+"in material library (MTL)"+std::to_string(assetId));
		return 0;
	}
	return mtl->getMaterial(materialId);
}
void AssetUtils::bindTexture(int assetId)
{
	Asset *asset = GET_ASSET(assetId);
	if(asset==0)
	{
		util::Globals::fatalError("No such texture asset "+std::to_string(assetId));
		return;
	}
	DDSImage *img = dynamic_cast<DDSImage *>(asset);
	if(img==0)
	{
		util::Globals::fatalError("Asset "+std::to_string(assetId)+" is not a texture");
		return;
	}
	img->bindTexture();
}
