#pragma once
#include <type_traits>
#include <algorithm>	

///////////////////////////////////////////////////////////////////////////////
template <typename Type>
Type Square(const Type& val)
{
	return (val * val);
}

///////////////////////////////////////////////////////////////////////////////
template <typename Type>
const Type& Clamp(const Type& value, const Type& min, const Type& max)
{
	return std::min(max, std::max(min, value));
}

///////////////////////////////////////////////////////////////////////////////
template <typename Type>
Type Lerp(const Type& start, const Type& end, float f)
{
	return static_cast<Type>(start + (end - start) * f);
}

///////////////////////////////////////////////////////////////////////////////
template <typename A, typename B>
B Remap(const A& a, const A& a1, const A& a2, const B& b1, const B& b2)
{
	float f = float(a - a1) / float(a2 - a1);
	return b1 + static_cast<B>((b2 - b1) * f);
}

///////////////////////////////////////////////////////////////////////////////
template <typename A, typename B>
B RemapClamped(const A& a, const A& a1, const A& a2, const B& b1, const B& b2)
{
	float f = float(a - a1) / float(a2 - a1);
	return f <= 0.f ? b1 : (f >= 1.f ? b2 : (b1 + static_cast<B>((b2 - b1) * f)));
}

///////////////////////////////////////////////////////////////////////////////
template <typename A, typename B>
B RemapClamped(const A& a, const A& a1, const A& a2, const B& b1, const B& b2, float power)
{
	float f = float(a - a1) / float(a2 - a1);
	return f <= 0.f ? b1 : (f >= 1.f ? b2 : (b1 + static_cast<B>((b2 - b1) * std::pow(f, power))));
}

///////////////////////////////////////////////////////////////////////////////
template <typename Type>
Type Sign(const Type& val)
{
	return (val < static_cast<Type>(0)) ? static_cast<Type>(-1) : static_cast<Type>(1);
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
Random(const T& min, const T& max)
{
	if (min == max)
	{
		return min;
	}
	if (max < min)
	{
		return max + rand() % (min - max);
	}
	return min + rand() % (max - min);
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
typename std::enable_if<!std::is_integral<T>::value, T>::type
Random(const T& min, const T& max)
{
	return static_cast<T>(((static_cast<float>(rand()) / RAND_MAX) * (max - min)) + min);
}

///////////////////////////////////////////////////////////////////////////////
template <typename Container, typename T>
typename Container::const_iterator Find(const Container& container, const T& value)
{
	return std::find(container.begin(), container.end(), value);
}

///////////////////////////////////////////////////////////////////////////////
template <typename Container, typename T>
typename Container::iterator Find(Container& container, const T& value)
{
	return std::find(container.begin(), container.end(), value);
}

///////////////////////////////////////////////////////////////////////////////
template <typename Container, typename Predicate>
typename Container::const_iterator FindIf(const Container& container, Predicate p)
{
	return std::find_if(container.begin(), container.end(), p);
}

///////////////////////////////////////////////////////////////////////////////
template <typename Container, typename Predicate>
typename Container::iterator FindIf(Container& container, Predicate p)
{
	return std::find_if(container.begin(), container.end(), p);
}

///////////////////////////////////////////////////////////////////////////////
template <typename Container, typename Value>
typename std::iterator_traits<typename Container::const_iterator>::difference_type Count(const Container& container, const Value& value)
{
	return std::count(container.begin(), container.end(), value);
}

///////////////////////////////////////////////////////////////////////////////
template <typename Container, typename Value>
typename std::iterator_traits<typename Container::iterator>::difference_type Count(Container& container, const Value& value)
{
	return std::count(container.begin(), container.end(), value);
}

///////////////////////////////////////////////////////////////////////////////
template <typename Container, typename Predicate>
typename std::iterator_traits<typename Container::const_iterator>::difference_type CountIf(const Container& container, Predicate predicate)
{
	return std::count_if(container.begin(), container.end(), predicate);
}

///////////////////////////////////////////////////////////////////////////////
template <typename Container, typename Predicate>
typename std::iterator_traits<typename Container::iterator>::difference_type CountIf(Container& container, Predicate predicate)
{
	return std::count_if(container.begin(), container.end(), predicate);
}

///////////////////////////////////////////////////////////////////////////////
template <typename Container, typename T>
bool Contains(Container& container, const T& value)
{
	return Find(container, value) != container.end();
}

///////////////////////////////////////////////////////////////////////////////
template <typename Container, typename Predicate>
Container CopyIf(const Container& from, Predicate p)
{
	Container result;
	std::copy_if(from.begin(), from.end(), std::back_inserter(result), p);
	return result;
}

///////////////////////////////////////////////////////////////////////////////
template <typename Container, typename Predicate>
void InsertionSort(Container& container, Predicate pred)
{
	for (int i = 1; i < (int)container.size(); i++)
	{
		for (int j = i; j > 0 && pred(container[j], container[j - 1]); j--)
		{
			std::swap(container[j], container[j - 1]);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
template <typename Container>
void InsertionSort(Container& container)
{
	for (int i = 1; i < (int)container.size(); i++)
	{
		for (int j = i; j > 0 && container[j] < container[j - 1]; j--)
		{
			std::swap(container[j], container[j - 1]);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
T Bezier(const T& a, const T& b, const T& c, float t)
{
	const float k = 1.f - t;
	return k * k * a + 2 * k * t * b + t * t * c;
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
T BezierTangent(const T& a, const T& b, const T& c, float t)
{
	const float k = 1.f - t;
	return 2 * k * (b - a) + 2 * t * (c - b);
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
float BezierLength(const T& a, const T& b, T& c, int samples = 10)
{
	float length = 0.0f;
	T prev = a;

	for (int i = 1; i <= samples; i++)
	{
		float current = float(i) / (samples + 1);
		T t = Bezier(a, b, c, current);
		length += (t - prev).getLength();
		prev = t;
	}

	length += (b - prev).getLength();
	return length;
};

///////////////////////////////////////////////////////////////////////////////
template <typename To, typename From>
typename std::enable_if < std::is_arithmetic<To>::value&& std::is_floating_point<From>::value, To >::type
Ceil(From from)
{
	return static_cast<To>(std::ceil(from));
}

///////////////////////////////////////////////////////////////////////////////
template <typename To, typename From>
typename std::enable_if < std::is_arithmetic<To>::value&& std::is_floating_point<From>::value, To >::type
Floor(From from)
{
	return static_cast<To>(std::floor(from));
}

///////////////////////////////////////////////////////////////////////////////
template <typename To, typename From>
typename std::enable_if < std::is_arithmetic<To>::value&& std::is_floating_point<From>::value, To >::type
Round(From from)
{
	return from < 0 ? static_cast<To>(std::ceil(from - (From)0.5)) : static_cast<To>(std::floor(from + (From)0.5));
}

///////////////////////////////////////////////////////////////////////////////
