#ifndef __ASSETMANAGER_H_INCLUDED__
#define __ASSETMANAGER_H_INCLUDED__

namespace util {
	class AssetManager;
	class Asset;
}
namespace render {
	class MD5Model;
}


#define ASSET_MTLLIB 0
#define ASSET_WAVEFRONT 1
#define ASSET_MD5MESH 2
#define ASSET_MD5ANIM 3

#include <mutex>
#include <list>
#include <thread>
#include <vector>
#include <ostream>

std::ostream &operator<<(std::ostream &ost, const util::Asset &asset);

namespace util {
	class AssetManager {
		public:
			static AssetManager *getAssetManager();
			void init();
			void run();
			bool postload();
			float getProgress();
		private:
			static AssetManager *instance;
			int progress_current;
			int progress_total;
			std::thread *assetManagerThread;
			std::mutex progress_mutex;
			std::list<render::MD5Model *> models;
			std::vector<Asset *> assets;
			bool preload_complete,postload_complete;
	};
	class Asset {
		public:
			Asset(int assetId);
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
}

#endif
