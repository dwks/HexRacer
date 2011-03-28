#ifndef PROJECT_MAP__MAP_OPTIONS_H
#define PROJECT_MAP__MAP_OPTIONS_H

#include <string>
#include <fstream>

#define HRMAP_BGBLOOMENABLE_LABEL "bgBloomEnable"
#define HRMAP_NUMLAPS_LABEL "numLaps"

namespace Project {
namespace Map {

class MapOptions {
private:
	bool bgBloomEnable;
	int numLaps;
public:
	MapOptions();
	void clear();

	bool getBGBloomEnable() const { return bgBloomEnable; }
	void setBGBloomEnable(bool enable) { bgBloomEnable = enable; }

	int getNumLaps() const { return numLaps; }
	void setNumLaps(int laps);

	bool parseStream(const std::string& keyword, std::ifstream& stream, const std::string& version);
	void saveToStream(std::ofstream& stream) const;
	
};

}  // namespace Map
}  // namespace Project

#endif
