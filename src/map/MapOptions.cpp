#include "MapOptions.h"


namespace Project {
namespace Map {

	MapOptions::MapOptions() {
		clear();
	}

	void MapOptions::clear() {
		bgBloomEnable = false;
		numLaps = 3;
	}

	bool MapOptions::parseStream(const std::string& keyword, std::ifstream& stream, const std::string& version) {
		if (keyword == HRMAP_BGBLOOMENABLE_LABEL) {
			stream >> bgBloomEnable;
			return true;
		}
		
		return false;
	}

	void MapOptions::saveToStream(std::ofstream& stream) const {

		stream << "#Options\n";
		stream << HRMAP_BGBLOOMENABLE_LABEL << ' ' << bgBloomEnable << '\n';

	}

}  // namespace Map
}  // namespace Project
