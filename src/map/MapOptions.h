#ifndef PROJECT_MAP__MAP_OPTIONS_H
#define PROJECT_MAP__MAP_OPTIONS_H

#include <string>
#include <fstream>

#define HRMAP_BGBLOOMENABLE_LABEL "bgBloomEnable"

namespace Project {
namespace Map {

class MapOptions {
private:
	bool bgBloomEnable;
public:
	MapOptions();
	void clear();
	bool getBGBloomEnable() const { return bgBloomEnable; }
	void setBGBloomEnable(bool enable) { bgBloomEnable = enable; }

	bool parseStream(const std::string& keyword, std::ifstream& stream, const std::string& version);
	void saveToStream(std::ofstream& stream) const;
	
};

}  // namespace Map
}  // namespace Project

#endif
