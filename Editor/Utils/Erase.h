#pragma once

#include <vector>
#include <deque>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

namespace erase_detail
{
	struct vectorlike_tag {};
	struct listlike_tag {};
	struct associative_tag {};

	template <typename Container>
	struct container_traits
	{
	};

	template <typename Type, typename Allocator>
	struct container_traits<std::vector<Type, Allocator>>
	{
		typedef vectorlike_tag category;
	};

	template <typename Type, typename Allocator>
	struct container_traits<std::deque<Type, Allocator>>
	{
		typedef vectorlike_tag category;
	};

	template <typename Type, typename Allocator>
	struct container_traits<std::list<Type, Allocator>>
	{
		typedef listlike_tag category;
	};

	template <typename Type, typename Comparator, typename Allocator>
	struct container_traits<std::set<Type, Comparator, Allocator>>
	{
		typedef associative_tag category;
	};

	template <typename Type, typename Comparator, typename Allocator>
	struct container_traits<std::multiset<Type, Comparator, Allocator>>
	{
		typedef associative_tag category;
	};

	template <typename Key, typename Value, typename Comparator, typename Allocator>
	struct container_traits<std::map<Key, Value, Comparator, Allocator>>
	{
		typedef associative_tag category;
	};

	template <typename Key, typename Value, typename Comparator, typename Allocator>
	struct container_traits<std::multimap<Key, Value, Comparator, Allocator>>
	{
		typedef associative_tag category;
	};

	template <typename Key, typename Value, typename Hasher, typename Comparator, typename Allocator>
	struct container_traits<std::unordered_map<Key, Value, Hasher, Comparator, Allocator>>
	{
		typedef associative_tag category;
	};


	// helpers
	template <typename Container, typename Type>
	void erase_helper(Container& c, const Type& value, vectorlike_tag)
	{
		c.erase(std::remove(c.begin(), c.end(), value), c.end());
	}

	template <typename Container, typename Predicate>
	void erase_if_helper(Container& c, Predicate p, vectorlike_tag)
	{
		c.erase(std::remove_if(c.begin(), c.end(), p), c.end());
	}

	template <typename Container, typename Type>
	void erase_helper(Container& c, const Type& value, listlike_tag)
	{
		c.remove(value);
	}

	template <typename Container, typename Predicate>
	void erase_if_helper(Container& c, Predicate p, listlike_tag)
	{
		c.remove_if(p);
	}

	template <typename Container, typename Type>
	void erase_helper(Container& c, Type value, associative_tag)
	{
		c.erase(value);
	}

	template <typename Container, typename Predicate>
	void erase_if_helper(Container& c, Predicate p, associative_tag)
	{
		for (typename Container::iterator i = c.begin(); i != c.end();)
		{
			if (p(*i))
			{
				c.erase(i++);
			}
			else
			{
				++i;
			}
		}
	}
}// end of namespace erase_detail

template <typename Container, typename Type>
void Erase(Container& c, const Type& value)
{
	erase_detail::erase_helper(c, value, typename erase_detail::container_traits<Container>::category());
}

template <typename Container, typename Predicate>
void EraseIf(Container& c, Predicate p)
{
	erase_detail::erase_if_helper(c, p, typename erase_detail::container_traits<Container>::category());
}

