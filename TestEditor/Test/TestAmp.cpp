#include <stdafx.h>
#include "Test.h"

#include <sstream>
#include <windows.h>
#include <amp.h>

//https://msdn.microsoft.com/en-us/library/hh265136.aspx

using namespace concurrency;

accelerator PickAccelerator()
{
	accelerator result;	// default contructor -> default accelerator

	// query all accelerators and pick one
	for (auto acc : accelerator::get_all())
	{
		if (!acc.has_display)
			result = acc;
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////
void DoIt(int* v, int size, int inc)
{
	// first version
	//extent<1> e(size);
	//array_view<int, 1> av(e, v);
	//parallel_for_each (e, [=](index<1> i) restrict(amp)
	//{
	//	av[i[0]] += inc;
	//});

	// second version
	array_view<int> av(size, v);
	parallel_for_each (av.extent, [=](index<1> idx) restrict(amp)
	{
		av[idx] += inc;
	});

	std::wostringstream ss;
	for (size_t i = 0; i < size; i++)
	{
		ss << static_cast<char>(av[i]);
	}
	OutputDebugString(ss.str().c_str());
}

///////////////////////////////////////////////////////////////////////////////
void DoIt_CPU(int* v, int size, int inc)
{
	for (int i = 0; i < size; i++)
	{
		v[i] += inc;
	}

	std::wostringstream ss;
	for (size_t i = 0; i < size; i++)
	{
		ss << static_cast<char>(v[i]);
	}
	OutputDebugString(ss.str().c_str());
}

///////////////////////////////////////////////////////////////////////////////
void TestAmp()
{
	int v[] = { 'G', 'd', 'k', 'k', 'n', 31, 'v', 'n', 'q', 'k', 'c', '-' };
	int sz = 12;
	int inc = 1;

	DoIt_CPU(v, sz, inc);
}

///////////////////////////////////////////////////////////////////////////////
