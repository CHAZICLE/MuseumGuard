#include <iostream>
#include <fstream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include "render/MD5Model.hpp"
#include "render/OBJModel.hpp"
#include "util/StreamUtils.hpp"
#include "render/MaterialLibrary.hpp"

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
void AssetManager::init()
{
	this->progress_current = 0;
	this->progress_total = 0;
	this->assetManagerThread = new std::thread(assetManagerThreadRun);
	this->preload_complete = false;
	this->postload_complete = false;
}
void AssetManager::run()
{
	std::cout << "AssetManager: Loader Thread started" << std::endl;
	//Open file with boost libs
	std::ifstream gzfile("assets.gz", std::ios_base::in | std::ios_base::binary);
	if(!gzfile.is_open()) {
		std::cout << "FAILED TO OPEN FILE" << std::endl;
		return;
	}
	
	boost::iostreams::filtering_streambuf<boost::iostreams::input> inbuf;
	inbuf.push(boost::iostreams::gzip_decompressor());
	inbuf.push(gzfile);
	std::istream fp(&inbuf);
	
	Asset *asset;
	int assetType = 0, assetId = 0;
	render::MaterialLibrary *mtlib = 0;
	render::MD5Model *md5Model = 0;
	render::OBJModel *wvModel = 0;
	while(!fp.eof())
	{
		fp.read((char *)&assetType, 1);
//		std::cout << assetType << std::endl;
		if(fp.eof())
			break;
		switch(assetType)
		{
			case ASSET_MTLLIB:
				mtlib = new render::MaterialLibrary(assetId, fp);
				asset = mtlib;
				break;
			case ASSET_WAVEFRONT:
				wvModel = new render::OBJModel(assetId, fp);
				asset = wvModel;
				break;
			case ASSET_MD5MESH:
				md5Model = new render::MD5Model(assetId, fp);
				asset = md5Model;
				break;
			case ASSET_MD5ANIM:
				break;
			default:
				std::cerr << "ERROR: Unknown asset type" << std::endl;
				return;
		}
		this->assets.push_back(asset);
		std::cout << *asset << std::endl;
		assetId++;
	}
	preload_complete = true;
	std::cout << "Reader thread exit" << std::endl;
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
Asset::Asset(int assetId)
{
	this->_assetId = assetId;
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
