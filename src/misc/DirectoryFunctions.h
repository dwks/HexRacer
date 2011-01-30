#ifndef PROJECT_MISC__DIRECTORY_FUNCTIONS_H
#define PROJECT_MISC__DIRECTORY_FUNCTIONS_H

#include <string>
using namespace std;

namespace Project {
namespace Misc {

class DirectoryFunctions {
public:

	static bool fileExists(const char *name);
	static string extractFilename(string str, bool include_extension = true);
	static string extractDirectory(string str);
	static string toRelativeFilename(string base_directory, string full_filename);
	static string fromRelativeFilename(string base_directory, string relative_filename);

};

}  // namespace Misc
}  // namespace Project

#endif
