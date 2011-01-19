#ifndef PROJECT_MISC__DIRECTORY_FUNCTIONS_H
#define PROJECT_MISC__DIRECTORY_FUNCTIONS_H

#include <string>
using namespace std;

namespace Project {
namespace Misc {

class DirectoryFunctions {
public:

	static string extractFilename(string str);
	static string extractDirectory(string str);
	static bool fileExists(const char *name);

};

}  // namespace Misc
}  // namespace Project

#endif
