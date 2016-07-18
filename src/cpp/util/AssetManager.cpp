#include <iostream>
#include <fstream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include "util/StreamUtils.hpp"
#include "util/Globals.hpp"
#include "render/MaterialLibrary.hpp"
#include "render/StaticModel.hpp"
#include "render/SkeletalModel.hpp"
#include "render/SkeletalAnimation.hpp"
#include "render/DDSImage.hpp"
#include "ai/path/NavigationGraph.hpp"

#include "AssetManager.hpp"

using namespace util;
using namespace util::StreamUtils;

void assetManagerThreadRun()
{
	AssetManager::getAssetManager()->run();
}

AssetManager *AssetManager::instance = 0;
AssetManager *AssetManager::getAssetManager()
{
	if(AssetManager::instance==0)
	{
		AssetManager::instance = new AssetManager();
	}
	return AssetManager::instance;
}
AssetManager::AssetManager()
{
	instance = 0;
	progress_current = 0;
	progress_total = 0;
	memset(assets, 0, sizeof(assets));
	assetManagerThread = 0;
	preload_complete = 0;
	postload_complete = 0;
}
AssetManager::~AssetManager()
{
	for(auto a : this->assets)
		delete a;
}
void AssetManager::init()
{
	this->assetManagerThread = new std::thread(assetManagerThreadRun);
}
void AssetManager::cleanup()
{
	this->assetManagerThread->join();
	delete this->assetManagerThread;
	delete AssetManager::instance;
}
void AssetManager::run()
{
	//Open file with boost libs
	std::ifstream gzfile("assets.gz", std::ios_base::in | std::ios_base::binary);
	if(!gzfile.is_open()) {
		std::cerr << "ERROR: Failed to open asset file" << std::endl;
		return;
	}
	
	boost::iostreams::filtering_streambuf<boost::iostreams::input> inbuf;
	inbuf.push(boost::iostreams::gzip_decompressor());
	inbuf.push(gzfile);
	std::istream fp(&inbuf);
	
	Asset *asset;
	int assetType = 0, assetId = 0;
	while(!fp.eof())
	{
		fp.read((char *)&assetType, 1);
		if(fp.eof())
			break;
		//std::cout << "READ TYPE: " << assetType << std::endl;
		switch(assetType)
		{
			case ASSET_MTLLIB:
				asset = new render::MaterialLibrary(assetId, fp);
				break;
			case ASSET_WAVEFRONT:
				asset = new render::StaticModel(assetId, fp);
				break;
			case ASSET_MD5MESH:
				asset = new render::SkeletalModel(assetId, fp);
				break;
			case ASSET_MD5ANIM:
				asset = new render::SkeletalAnimation(assetId, fp);
				break;
			case ASSET_DDS:
				asset = new render::DDSImage(assetId, fp);
				break;
			case ASSET_NAVMESH:
				asset = new ai::path::NavigationGraph(assetId, fp);
				break;
			default:
				util::Globals::fatalError("Unknown asset type "+std::to_string(assetType));
				return;
		}
		this->assets[assetId] = asset;
		assetId++;
	}
	preload_complete = true;
}
bool AssetManager::postload()
{
	if(postload_complete)
		return true;
	if(!preload_complete)
		return false;
	for(Asset *a : this->assets)
	{
		a->postload();
	}
	postload_complete = true;
	return true;
}
float AssetManager::getProgress()
{
	progress_mutex.lock();
	if(progress_total<=0)
	{
		return 0.f;
	}
	float f = (float)progress_current/(float)progress_total;
	progress_mutex.unlock();
	return f;
}
Asset *AssetManager::getAsset(int assetId)
{
	return this->assets[assetId];
}
Asset::Asset(int assetId)
{
	this->_assetId = assetId;
}
Asset::~Asset()
{
	
}
int Asset::getAssetID() const
{
	return this->_assetId;
}
std::string Asset::getName() const
{
	return this->name;
}
void Asset::setName(std::string name)
{
	this->name = name;
}
void Asset::write(std::ostream &ost) const
{
	ost << "[" << getAssetID() << ":" << getName() << "]";
}
std::ostream &operator<<(std::ostream &ost, const Asset &asset)
{
	asset.write(ost);
	return ost;
}
