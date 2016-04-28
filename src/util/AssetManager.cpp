#include <thread>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include "AssetManager.hpp"

using namespace util::AssetManager;

void init()
{
	this->progress_current = 0;
	this->progress_total = 0;
	std::thread loaderThread(run, "AssetManager: Loader Thread");
}
void run()
{
	//Open file with boost libs
	std::ifstream gzfile("data.gz", std::ios_base::in | std::ios_base::binary);
	if(!gzfile.is_open()) {
		std::cerr << "FAILED TO OPEN FILE" << std::endl;
	}
	
	boost::iostreams::filtering_streambuf<boost::iostreams::input> inbuf;
	inbuf.push(boost::iostreams::gzip_decompressor());
	inbuf.push(gzfile);
	std::istream fp(&inbuf);

	while(true) {
		
	}
}
float getProgress()
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
