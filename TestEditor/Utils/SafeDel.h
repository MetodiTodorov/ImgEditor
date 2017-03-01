#pragma once

#include <vector>
#include <deque>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

namespace safedel_detail
{
	struct ptr_level1_tag {};
	struct standart_container_tag {};

	template <typename T>
	struct ptr_traits
	{
	};

	template<typename T>
	struct ptr_traits<T*>
	{
		typedef ptr_level1_tag category;
	};

	template<typename T>
	struct ptr_traits<T**>
	{
	};

	template<typename T>
	struct ptr_traits<T***>
	{
	};

	template<typename T, typename Allocator>
	struct ptr_traits<std::vector<T, Allocator>>
	{
		typedef standart_container_tag category;
	};

	template<typename T, typename Allocator>
	struct ptr_traits<std::deque<T, Allocator>>
	{
		typedef standart_container_tag category;
	};

	template<typename T, typename Comparator, typename Allocator>
	struct ptr_traits<std::set<T, Comparator, Allocator>>
	{
		typedef standart_container_tag category;
	};

	template<typename T, typename Allocator>
	struct ptr_traits<std::list<T, Allocator>>
	{
		typedef standart_container_tag category;
	};

	template<typename T>
	void SafeDel_helper(T& ptr, ptr_level1_tag)
	{
		if (ptr)
		{
			delete ptr;
			ptr = nullptr;
		}
	}

	template<typename T>
	void SafeDel_NonZerohelper(T& ptr, ptr_level1_tag)
	{
		if (ptr)
		{
			delete ptr;
		}
	}

	template<typename T>
	void SafeDel_helper(T& container, standart_container_tag)
	{
		typedef typename T::iterator container_iterator;
		typedef typename container_iterator::value_type type;

		for (container_iterator it = container.begin(); it != container.end(); ++it)
		{
			SafeDel_NonZerohelper(*it, typename ptr_traits<type>::category());
		}

		container.clear();
	}

}

template <typename T>
void SafeDel(T& ptr)
{
	safedel_detail::SafeDel_helper(ptr, typename safedel_detail::ptr_traits<T>::category());
}

template <typename T>
void SafeDelArray(T& ptr)
{
	if (ptr)
	{
		delete [] ptr;
		ptr = nullptr;
	}
}

template <typename T>
void SafeDelMap(T& container)
{
	typedef typename T::mapped_type type;

	for (auto& it : container)
	{
		safedel_detail::SafeDel_NonZerohelper(it.second, typename safedel_detail::ptr_traits<type>::category());
	}

	container.clear();
}

