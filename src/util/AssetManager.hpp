#ifndef 3YP_UTIL_ASSETMANAGER_HPP_
#define 3YP_UTIL_ASSETMANAGER_HPP_

namespace util {
	class AssetManager;
	class Asset;
}
namespace render {
	class SkeletalModel;
}


#define ASSET_MTLLIB 0
#define ASSET_WAVEFRONT 1
#define ASSET_MD5MESH 2
#define ASSET_MD5ANIM 3
#define ASSET_DDS 4
#define ASSET_NAVMESH 5

#include <mutex>
#include <list>
#include <thread>
#include <vector>
#include <ostream>
#include "util/AssetsMeta.h"

std::ostream &operator<<(std::ostream &ost, const util::Asset &asset);

namespace util {
	class Asset {
		public:
			Asset(int assetId);
			virtual ~Asset();
			int getAssetID() const;
			std::string getName() const;
			virtual void write(std::ostream &ost) const;
			virtual void postload() = 0;
		private:
			int _assetId;
			std::string name;
		protected:
			void setName(std::string name);
	};
	class AssetManager {
		public:
			AssetManager();
			~AssetManager();
			static AssetManager *getAssetManager();
			void init();
			void cleanup();
			void run();
			bool postload();
			float getProgress();
			Asset *getAsset(int assetId);
		private:
			static AssetManager *instance;
			int progress_current;
			int progress_total;
			std::thread *assetManagerThread;
			std::mutex progress_mutex;
			Asset *assets[ASSETS_COUNT];
			bool preload_complete,postload_complete;
	};
}

#endif
