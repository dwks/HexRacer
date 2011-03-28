#include "StringTextureCache.h"

namespace Project {
namespace Render {

	StringTextureCache *StringTextureCache::instance = 0;

	StringTextureCache::StringTextureCache() {
		instance = this;
	}

	StringTextureCache::~StringTextureCache() {
		clear();
	}

	StringTexture* StringTextureCache::getStringTexture(const std::string& str, TTF_Font *font) {

		StringTextureInfo info;
		info.font = font;
		info.string = std::string(str);

		std::map<StringTextureInfo, StringTexture*>::iterator found = cache.find(info);
		if (found == cache.end()) {
			StringTexture* new_string_tex = new StringTexture();
			new_string_tex->setString(str, font);
			cache[info] = new_string_tex;
			return new_string_tex;
		}
		else
			return (*found).second;
	}

	void StringTextureCache::clear() {

		for (std::map<StringTextureInfo, StringTexture*>::iterator it = cache.begin();
			it != cache.end(); ++it) {
			delete((*it).second);
		}
		cache.clear();

	}

}  // namespace Render
}  // namespace Project
