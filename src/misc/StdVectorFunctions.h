#ifndef PROJECT_MISC__STD_VECTOR_FUNCTIONS_H
#define PROJECT_MISC__STD_VECTOR_FUNCTIONS_H

#include <vector>
#include <stack>

namespace Project {
namespace Misc {

template <typename Type>
static void vectorRemoveAtIndex(std::vector<Type>& v, int index) {
	v[index] = v[v.size()-1];
	v.resize(v.size()-1);
}

template <typename Type>
static bool vectorRemoveOneElement(std::vector<Type>& v, const Type& element) {
	for (unsigned int i = 0; i < v.size(); i++) {
		if (v[i] == element) {
			vectorRemoveAtIndex(v, i);
			return true;
		}
	}

	return false;
}

template <typename Type>
static bool vectorContains(const std::vector<Type>& v, const Type& element) {
	for (unsigned int i = 0; i < v.size(); i++) {
		if (v[i] == element) {
			return true;
		}
	}
	return false;
}

template <typename Type>
static void vectorAppend(std::vector<Type>& v1, const std::vector<Type>& v2) {
	for (unsigned int i = 0; i < v2.size(); i++)
		v1.push_back(v2[i]);
}

template <typename Type>
static std::vector<Type> vectorAppended(std::vector<Type>& v1, const std::vector<Type>& v2) {

	std::vector<Type> return_v(v1);
	return_v.insert(return_v.end(), v2.begin(), v2.end());
	return return_v;

}

template <typename Type>
static int vectorIndexOfFirst(std::vector<Type>& v, const Type& element) {

	for (unsigned int i = 0; i < v.size(); i++) {
		if (v[i] == element) {
			return static_cast<int>(i);
		}
	}
	return -1;

}

template <typename Type>
static void vectorMergeSort(std::vector<Type>& v, int start_index = -1, int end_index = -1) {

	if (start_index < 0)
		start_index = 0;
	if (end_index < 0)
		end_index = v.size()-1;

	int size = end_index-start_index;
	size++;

	if (size < 2)
		return;
	
	if (size == 2) {
		//Trivial sort of a size 2 vector
		if (v[end_index] < v[start_index]) {
			Type temp = v[start_index];
			v[start_index] = v[end_index];
			v[end_index] = temp;
		}
		return;
	}

	int split_point = (start_index+end_index)/2;
	int start_index2 = split_point;
	start_index2++;

	//Sort the two halves of the vector
	vectorMergeSort(v, start_index, split_point);
	vectorMergeSort(v, start_index2, end_index);

	//Merge
	std::stack<Type> buffer1;
	for (int i = split_point; i >= start_index; i--)
		buffer1.push(v[i]);

	std::stack<Type> buffer2;
	for (int i = end_index; i >= start_index2; i--)
		buffer2.push(v[i]);
	
	for (int i = 0; i < size; i++) {
		if (!buffer1.empty() && (buffer2.empty() || buffer1.top() < buffer2.top())) {
			v[start_index+i] = buffer1.top();
			buffer1.pop();
		}
		else {
			v[start_index+i] = buffer2.top();
			buffer2.pop();
		}
	}

}

template <typename Type>
static void vectorPointerMergeSort(std::vector<Type>& v, int start_index = -1, int end_index = -1) {

	if (start_index < 0)
		start_index = 0;
	if (end_index < 0)
		end_index = v.size()-1;

	int size = end_index-start_index;
	size++;

	if (size < 2)
		return;
	
	if (size == 2) {
		//Trivial sort of a size 2 vector
		if (*v[end_index] < *v[start_index]) {
			Type temp = v[start_index];
			v[start_index] = v[end_index];
			v[end_index] = temp;
		}
		return;
	}

	int split_point = (start_index+end_index)/2;
	int start_index2 = split_point;
	start_index2++;

	//Sort the two halves of the vector
	vectorPointerMergeSort(v, start_index, split_point);
	vectorPointerMergeSort(v, start_index2, end_index);

	//Merge
	std::stack<Type> buffer1;
	for (int i = split_point; i >= start_index; i--)
		buffer1.push(v[i]);

	std::stack<Type> buffer2;
	for (int i = end_index; i >= start_index2; i--)
		buffer2.push(v[i]);
	
	for (int i = 0; i < size; i++) {
		if (!buffer1.empty() && (buffer2.empty() || *(buffer1.top()) < *(buffer2.top()))) {
			v[start_index+i] = buffer1.top();
			buffer1.pop();
		}
		else {
			v[start_index+i] = buffer2.top();
			buffer2.pop();
		}
	}

}

}  // namespace Misc
}  // namespace Project

#endif
