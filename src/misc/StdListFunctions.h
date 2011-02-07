#ifndef PROJECT_MISC__STD_LIST_FUNCTIONS_H
#define PROJECT_MISC__STD_LIST_FUNCTIONS_H

#include <list>

namespace Project {
namespace Misc {

	template <typename Type>
	static bool listRemoveAll(const std::list<Type>& list, const Type& element) {
		int size = list.size();
		list.remove(element);
		return (size != list.size());
	}

	template <typename Type>
	static bool listContains(const std::list<Type>& list, const Type& element) {
		for (std::list::const_iterator iterator = list.begin(); iterator != list.end(); ++iterator) {
			if (*iterator == element)
				return true;
		}

		return false;
	}

}  // namespace Misc
}  // namespace Project

#endif
