#ifndef PROJECT_MISC__DIRECTORY_FUNCTIONS_H
#define PROJECT_MISC__DIRECTORY_FUNCTIONS_H

#include <string>
using namespace std;

namespace Project {
namespace Misc {

class DirectoryFunctions {
public:

	static bool fileExists(const char *name);
	static string extractFilename(const string& str, bool include_extension = true);
	static string extractDirectory(const string& str);
	static string toRelativeFilename(const string& base_directory, const string& full_filename);
	static string fromRelativeFilename(const string& base_directory, const string& relative_filename);

};

}  // namespace Misc
}  // namespace Project

#endif
