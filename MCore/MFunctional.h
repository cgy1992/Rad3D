/*
*	Functional
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MDefine.h"

namespace Rad {

	template <class T>
	struct t_compare
	{
		int operator ()(const T & a, const T & b)
		{
			return a < b ? -1 : ((a > b) ? 1 : 0);
		}
	};

	template <class T>
	struct t_less
	{
		bool operator ()(const T & a, const T & b)
		{
			return a < b;
		}
	};

	template <class T>
	struct t_less_equal
	{
		bool operator ()(const T & a, const T & b)
		{
			return a <= b;
		}
	};

	template <class T>
	struct t_greater
	{
		bool operator ()(const T & a, const T & b)
		{
			return a > b;
		}
	};

	template <class T>
	struct t_greater_equal
	{
		bool operator ()(const T & a, const T & b)
		{
			return a >= b;
		}
	};

	template <class T>
	struct t_equal
	{
		bool operator ()(const T & a, const T & b)
		{
			return a == b;
		}
	};

	template <class T>
	struct t_not_equal_to
	{
		bool operator ()(const T & a, const T & b)
		{
			return a != b;
		}
	};

	template <class T>
	struct t_logical_and
	{
		bool operator ()(const T & a, const T & b)
		{
			return a && b;
		}
	};

	template <class T>
	struct t_logical_all
	{
		bool operator ()(const T & a, const T & b)
		{
			return a || b;
		}
	};

	template <class T>
	struct t_hash
	{ 
		int operator()(T x) const
		{ 
			return (int)x;
		}
	};
	
	// Swap
	//
	template<class T>
	inline void Swap(T & a, T & b)
	{
		T temp = a;
		a = b;
		b = temp;
	}

	// Min & Max
	//
	template<class T>
	inline const T & Max(const T & a, const T & b)
	{
		return a > b ? a : b;
	}

	template<class T>
	inline const T & Max3(const T & a, const T & b, const T & c)
	{
		return a > b ? (a > c ? a : c) : (b > c ? b : c);
	}
	
	template<class T>
	inline const T & Min(const T & a, const T & b)
	{
		return a < b ? a : b;
	}

	template<class T>
	inline const T & Min3(const T & a, const T & b, const T & c)
	{
		return a < b ? (a < c ? a : c) : (b < c ? b : c);
	}

	template<class T>
	inline int MaxIndex(const T & a, const T & b)
	{
		return a > b ? 0 : 1;
	}

	template<class T>
	inline int Max3Index(const T & a, const T & b, const T & c)
	{
		return a > b ? (a > c ? 0 : 2) : (b > c ? 1 : 2);
	}

	template<class T>
	inline int MinIndex(const T & a, const T & b)
	{
		return a < b ? 0 : 1;
	}

	template<class T>
	inline int Min3Index(const T & a, const T & b, const T & c)
	{
		return a < b ? (a < c ? 0 : 2) : (b < c ? 1 : 2);
	}

	// Bubble Sort
	//
	template <class T, class Cmp>
	void BubbleSort(T * arr, int count, Cmp & cmp)
	{
		int i, j, p;

		for (i = 0; i < count - 1; ++i)
		{
			p = i;
			for (j = i + 1; j < count; ++j)
			{
				if (cmp(arr[j], arr[p]) < 0)
					p = j;
			}

			T a = arr[i];
			arr[i] = arr[p];
			arr[p] = a;
		}
	}


	// Insert Sort
	//
	template <class T, class Cmp>
	void InsertSort(T * arr, int count, Cmp & compare)
	{
		int i, j;

		for(i = 1; i < count; ++i)
		{
			T tmp = arr[i];
        
			for (j = i; j > 0 && compare(tmp, arr[j -  1]) < 0; --j)
			{
				arr[j] = arr[j - 1];
			}

			arr[j] = tmp;
		}
	}

	// Heap Sort
	//
	template <class T, class Cmp>
	void HeapSort(T * arr, int count, Cmp & compare, T * pool)
	{
		T * heap = pool;
		int hp_size = 0;

		for (int i = 0; i < count; ++i)
		{
			T val = arr[i];
			int start = 0, end = hp_size - 1;

			while (start <= end)
			{
				int mid = (start + end) / 2;

				int cmp = compare(val, heap[mid]);
				if (cmp < 0)
				{
					end = mid - 1;
				}
				else if (cmp > 0)
				{
					start = mid + 1;
				}
				else
				{
					start = mid;
					break;
				}
			}

			int j = hp_size;
			while (j > start)
			{
				heap[j] = heap[j - 1];
				--j;
			}

			heap[start] = val;
			hp_size += 1;
		}

		for (int i = 0; i < count; ++i)
		{
			arr[i] = heap[i];
		}
	}

	template <class T, class Cmp, int POOL_SIZE>
	void HeapSortT(T * arr, int count, Cmp & compare)
	{
		d_assert(arr);

		T heap[POOL_SIZE];
		HeapSort(arr, count, compare, heap);
	}

	// Shell Sort
	//
	/*
	template <class T, class Cmp>
	void ShellSort(T * arr, int count, Cmp & compare)
	{
		d_assert (arr);

		int h = 1;

		while ((h * 3 + 1) < count) 
		{
			h = 3 * h + 1;
		}

		while( h > 0 )
		{
			int i, j;

			for (i = h - 1; i < count; i++) 
			{
				T b = arr[i];
				j = i;

				for(j = i; (j >= h) && compare(b, arr[j - h]) < 0; j -= h) 
				{
					arr[j] = arr[j-h];
				}

				arr[j] = b;
			}

			h = h / 3;
		}
	}
	*/


	// Merge Sort
	//
	/*
	template <class T, class Cmp, class Alloc>
	void __Merge(T * arr, int l, int m, int r, Cmp & compare, Alloc & alloc_)
	{
		int beg1 = l;
		int end1 = m;
		int beg2 = m + 1;
		int end2 = r;

		T * tmp = alloc_.alloc(r - l + 1);

		int k = 0;
		while (beg1 <= end1 && beg2 <= end2)
		{
			if (compare(arr[beg1], arr[beg2]) < 0)
			{
				tmp[k++] = arr[beg1++];
			}
			else
			{
				tmp[k++] = arr[beg2++];
			}
		}

		while (beg1 <= end1)
			tmp[k++] = arr[beg1++];

		while (beg2 <= end2)
			tmp[k++] = arr[beg2++];

		for (int i = 0; i < (r - l + 1); ++i)
		{
			arr[r + i] = tmp[i]; 
		}

		alloc_.free(tmp, r - l + 1);
	}


	template <class T, class Cmp, class Alloc>
	void __MergeSort(T * arr, int left, int right, Cmp & compare, Alloc & _alloc)
	{
		if (left < right)
		{
			int mid = (left + right) / 2;
			__MergeSort(arr, left, mid);
			__MergeSort(arr, mid + 1, right);
			__Merge(arr, left, mid, right);
		}
	}

	template <class T, class Cmp, class Alloc>
	void MergeSort(T * arr, int count, Cmp & compare, Alloc & _alloc)
	{
		__MergeSort(arr, 0, count - 1, compare, _alloc);
	}
	*/

	// Quick Sort
	//
	template <class T, class Cmp>
	int __QPartitions(T * a, int first, int last, Cmp & compare)
	{
		// if a[i] == a[i + 1], sometime we need keep their position
#if 0
		int i = first + rand() % (last - first + 1);
		Swap(a[first], a[i]);
#endif

		T pivotkey = a[first];

		while (first < last)
		{
			while (first < last && compare(a[last], pivotkey) > 0)
			{
				--last;
			}

			a[first] = a[last];

			while (first < last && compare(a[first], pivotkey) <= 0)
			{
				++first;
			}

			a[last] = a[first];
		}

		a[first] = pivotkey;
		return first;
	}

	template <class T, class Cmp>
	void __QSort(T * a, int first, int last, Cmp & compare)
	{
		int pivottag = __QPartitions(a, first, last, compare);

		if (first < pivottag - 1)
			__QSort(a, first, pivottag - 1, compare);

		if (pivottag + 1 < last)
			__QSort(a, pivottag + 1, last, compare); 
	}

	template <class T, class Cmp>
	void QuickSort(T * arr, int count, Cmp & compare)
	{
		assert (arr);

		if (count > 0)
		{
			__QSort(arr, 0, count - 1, compare);
		}
	}


	// Sort:
	//		Implement QuickSort and InsertSort
	//
	#define __sort_factor 20

	template <class T, class Cmp>
	void __Sort(T * a, int first, int last, Cmp & compare)
	{
		int count = last - first  + 1;

		if (count < __sort_factor)
			InsertSort(a + first, count, compare);
		else
		{
			int pivottag = __QPartitions(a, first, last, compare);

			if (first < pivottag - 1)
				__Sort(a, first, pivottag - 1, compare);

			if (pivottag + 1 < last)
				__Sort(a, pivottag + 1, last, compare); 
		}
	}

	template <class T, class Cmp>
	void Sort(T * arr, int count, Cmp & compare)
	{
		d_assert (arr);

		if (count > 0)
		{
			__Sort(arr, 0, count - 1, compare);
		}
	}

}