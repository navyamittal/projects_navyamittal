#pragma once

#include <functional> // std::less
#include <iterator> // std::iterator_traits

namespace sort {

	// This is C++ magic which will allows our function
	// to default to using a < b if the comparator arg
	// is unspecified. It uses defines std::less<T>
	// for the iterator's value_type.
	//
	// For example: if you have a vector<float>, the 
	// iterator's value type will be float. std::less 
	// will select the < for sorting floats as the 
	// default comparator.
	template<typename RandomIter>
	using less_for_iter = std::less<typename std::iterator_traits<RandomIter>::value_type>;

	/* Efficiently swap two items - use this to implement your sorts */
	template<typename T>
	void swap(T & a, T & b) noexcept {
		T temp;
		temp=std::move(a);
		a=std::move(b);
		b=std::move(temp);
		// a = std::move(b);
	}
/*Worst/Average Case: O(n²)
Best Case: O(n)


Each interaction compares the element with the element above it
If the element above is smaller, swap places
Compare the next element with the one after it
Goes through the array this way until no swaps are performed for an entire loop
*/
	template<typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void bubble(RandomIter begin, RandomIter end, Comparator comp = Comparator{}) {
		// Random access iterators have the same traits you defined in the Vector class
		// For instance, difference_type represents an iterator difference
		// You may delete the types you don't use to remove the compiler warnings comp(a,b)
		using _it             = std::iterator_traits<RandomIter>;
		using difference_type = typename _it::difference_type;
		
		difference_type diff = end - begin;
		for (int k = 1; k < diff; k++){
			bool cont = false;
			for (RandomIter j = begin; j < end-k; j++){
				if (comp(*(j+1), *j)){
					swap(*j, *(j+1));
					cont = true;
				}
			
			}
			if (!cont) break;
		}
	}
/*Worst/Average Case: O(n²)
Best Case: O(n)


Determines correct index for each element in comparison to the elements before it
Shifts the remaining sorted elements right
Iterates to compare the sorted elements with the next unsorted element
*/
	template<typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void insertion(RandomIter begin, RandomIter end, Comparator comp = Comparator{}) {
		for (RandomIter i = begin + 1; i < end; ++i) {
        //double temp = *i;
        RandomIter j = i - 1;

        while (j >= begin && comp(*(j+1), *j)) {
			swap(*(j+1), *j);
            j--;
        }
        //*(j + 1) = temp;
    	}
	}
/*Worst, Average, and Best Case: O(n²)


Selects minimum element during each iteration
Puts that element in the correct place, swapping with current element in that location
Repeats with remaining elements

Takes n(n-1)/2 comparisons
*/
	template<typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void selection(RandomIter begin, RandomIter end, Comparator comp = Comparator{}) {
		for (RandomIter k = begin; k < end; k++){
			RandomIter index = k;
			 for (RandomIter i = k + 1; i < end; i++){
			 	if (comp(*i, *index)) {
			 		index = i;
		 	}
		 }
		 	swap(*k, *index);
		}
	}
}
