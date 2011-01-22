#ifndef PROJECT_MISC__STD_VECTOR_FUNCTIONS_H
#define PROJECT_MISC__STD_VECTOR_FUNCTIONS_H

#include <vector>

namespace Project {
namespace Misc {

template <typename Type>
static void vectorRemoveAtIndex(std::vector<Type>& v, int index) {
	v[index] = v[v.size()-1];
	v.resize(v.size()-1);
}

template <typename Type>
static bool vectorRemoveOneElement(std::vector<Type>& v, Type element) {
	for (unsigned int i = 0; i < v.size(); i++) {
		if (v[i] == element) {
			vectorRemoveAtIndex(v, i);
			return true;
		}
	}

	return false;
}

template <typename Type>
static bool vectorContains(std::vector<Type>& v, Type element) {
	for (unsigned int i = 0; i < v.size(); i++) {
		if (v[i] == element) {
			return true;
		}
	}
	return false;
}

}  // namespace Misc
}  // namespace Project

#endif
