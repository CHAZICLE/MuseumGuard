#ifndef __ASSETMANAGER_H_INCLUDED__
#define __ASSETMANAGER_H_INCLUDED__

namespace util {
	class AssetManager;
}

#include <mutex>

namespace util {
	class AssetManager {
		public:
			static AssetManager *getAssetManager();
			void init();
			void run();
			float getProgress();
		private:
			int progress_current;
			int progress_total;
			std::mutex progress_mutex;
			
	};
}

#endif
