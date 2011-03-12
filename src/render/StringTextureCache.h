#ifndef PROJECT_RENDER__STRING_TEXTURE_CACHE_H
#define PROJECT_RENDER__STRING_TEXTURE_CACHE_H

#include <map>
#include "StringTexture.h"
#include "SDL_ttf.h"

namespace Project {
namespace Render {

class StringTextureCache {

private:
    static StringTextureCache *instance;
public:
    static StringTextureCache *getInstance() { return instance; }

private:

	struct StringTextureInfo {
		TTF_Font* font;
		std::string string;

		bool operator == (const StringTextureInfo& other) const {
			return (font == other.font && string == other.string);
		}

		bool operator < (const StringTextureInfo& other) const {
			return (font < other.font || string < other.string);
		}
	};

	std::map<StringTextureInfo, StringTexture*> cache;

public:

	StringTextureCache();
	~StringTextureCache();

	StringTexture* getStringTexture(const std::string& str, TTF_Font *font);
	void clear();
};

}  // namespace Render
}  // namespace Project

#endif
