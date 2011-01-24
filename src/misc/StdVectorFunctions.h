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

template <typename Type>
static void vectorAppend(std::vector<Type>& v1, std::vector<Type>& v2) {
	for (unsigned int i = 0; i < v2.size(); i++)
		v1.push_back(v2[i]);
}

template <typename Type>
static std::vector<Type> vectorAppended(std::vector<Type>& v1, std::vector<Type>& v2) {

	std::vector<Type> return_v(v1);
	for (unsigned int i = 0; i < v2.size(); i++)
		return_v.push_back(v2[i]);
	return return_v;

}

}  // namespace Misc
}  // namespace Project

#endif
