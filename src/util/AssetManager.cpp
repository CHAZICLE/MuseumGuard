#include <iostream>
#include <fstream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include "render/MD5Model.hpp"
#include "render/WavefrontModel.hpp"
#include "util/StreamUtils.hpp"

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
	
	int assetType = 0, assetId = 0;
	render::MD5Model *md5Model = 0;
	render::WavefrontModel *wvModel = 0;
	while(!fp.eof())
	{
		assetType = readInt(fp);
		std::cout << "Read a " << assetType << std::endl;
		switch(assetType)
		{
			case ASSET_WAVEFRONT:
				wvModel = new render::WavefrontModel(assetId, fp);
				break;
			case ASSET_MTLLIB:
				break;
			case ASSET_MD5MESH:
				md5Model = new render::MD5Model(assetId, fp);
				break;
			case ASSET_MD5ANIM:
				break;
			case ASSET_FONT:
				break;
			case ASSET_DDS:
				break;
		}
		assetId++;
		return;
	}
	std::cout << "Reader thread exit" << std::endl;
	
	// TODO: Read assets
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
int Asset::getAssetID()
{
	return this->_assetId;
}
std::string Asset::getName()
{
	return this->name;
}
void Asset::setName(std::string name)
{
	this->name = name;
}
