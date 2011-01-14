#include "DirectoryFunctions.h"
#include "math/Values.h"
using namespace Project;
using namespace Math;
using namespace std;

namespace Project {
namespace Misc {

	string DirectoryFunctions::extractFilename(string str) {
		int slashpos = str.find_last_of('/');
		if (slashpos == str.npos || slashpos == str.length()-1)
			return str;
		else {
			return str.substr(slashpos+1, str.length()-slashpos-1);
		}
	}

	string DirectoryFunctions::extractDirectory(string str) {
		return (str.substr(0, maximum((int) str.find_last_of('/')+1, 0)));
	}

}  // namespace Misc
}  // namespace Project
