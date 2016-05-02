#include <iostream>
#include <fstream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include "render/MD5Model.hpp"

#include "AssetManager.hpp"

using namespace util;

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
	
	char c = 0;
	render::MD5Model *model = 0;
	while(!fp.eof())
	{
		fp.read(&c, 1);
		std::cout << "Read a " << (int)c << std::endl;
		switch((int)c)
		{
			case ASSET_WAVEFRONT:
				break;
			case ASSET_MTLLIB:
				break;
			case ASSET_MD5MESH:
				model = new render::MD5Model(fp);
				break;
			case ASSET_MD5ANIM:
				break;
			case ASSET_FONT:
				break;
			case ASSET_DDS:
				break;
		}
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
