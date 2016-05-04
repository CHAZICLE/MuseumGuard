#ifndef __ASSETMANAGER_H_INCLUDED__
#define __ASSETMANAGER_H_INCLUDED__

namespace util {
	class AssetManager;
}
namespace render {
	class MD5Model;
}

#define ASSET_WAVEFRONT 0
#define ASSET_MTLLIB 1
#define ASSET_MD5MESH 2
#define ASSET_MD5ANIM 3
#define ASSET_FONT 4
#define ASSET_DDS 5

#include <mutex>
#include <list>
#include <thread>

namespace util {
	class AssetManager {
		public:
			static AssetManager *getAssetManager();
			void init();
			void run();
			float getProgress();
		private:
			static AssetManager *instance;
			int progress_current;
			int progress_total;
			std::thread *assetManagerThread;
			std::mutex progress_mutex;
			std::list<render::MD5Model *> models;
			
	};
	class Asset {
		public:
			Asset(int assetId);
			int getAssetID();
			std::string getName();
			virtual void postload() = 0;
		private:
			int _assetId;
			std::string name;
		protected:
			void setName(std::string name);
	};
}

#endif
