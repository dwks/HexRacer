#ifndef PROJECT_RENDER__SHADER_ATTRIBUTE_LOCATION_H
#define PROJECT_RENDER__SHADER_ATTRIBUTE_LOCATION_H

#include <string>
using namespace std;

namespace Project {
namespace Render {

class ShaderAttributeLocation {
public:

	string name;
	int location;

	ShaderAttributeLocation(string _name, int _location)
		: name(_name), location(_location) {}
	
};

}  // namespace Render
}  // namespace Project

#endif
