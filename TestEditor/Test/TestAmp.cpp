#include <stdafx.h>
#include "Test.h"

#include <sstream>
#include <windows.h>
#include <amp.h>

//https://msdn.microsoft.com/en-us/library/hh265136.aspx
//https://channel9.msdn.com/Blogs/DanielMoth/Matrix-Multiplication-with-C-AMP
//https://blogs.msdn.microsoft.com/nativeconcurrency/2012/08/30/learn-c-amp/

using namespace concurrency;

///////////////////////////////////////////////////////////////////////////////
// Basic AMP operations
///////////////////////////////////////////////////////////////////////////////
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
// AMP Tiling
///////////////////////////////////////////////////////////////////////////////
void MatMul_Serial(std::vector<int>& vC, const std::vector<int>& vA, const std::vector<int>& vB, int M, int N, int W)
{
	for (int row = 0; row < M; row++)
		for (int col = 0; col < N; col++)
		{
			int sum = 0;
			for (int k = 0; k < W; k++)
				sum += vA[row * W + k] + vB[k * N + col];
			vC[row * N + col] = sum;
		}
}

///////////////////////////////////////////////////////////////////////////////
void MatMul_AMP(std::vector<int>& vC, const std::vector<int>& vA, const std::vector<int>& vB, int M, int N, int W)
{
	array_view<const int, 2> a(M, W, vA);
	array_view<const int, 2> b(W, N, vB);
	array_view<int, 2> c(M, N, vC);
	c.discard_data();	// do not transfer to GPU

	//for (int row = 0; row < M; row++)
	//	for (int col = 0; col < N; col++)
	parallel_for_each(c.extent, [=](index<2> idx) restrict(amp)
	{
		int row = idx[0];
		int col = idx[1];

		int sum = 0;
		for (int k = 0; k < W; k++)
			sum += a(row, k) + b(k, col);
		c(idx) = sum;
	});

	c.synchronize(); // transfer back
}

///////////////////////////////////////////////////////////////////////////////
static const int TS = 2; // tile size
void MatMul_AMP_tiled(std::vector<int>& vC, const std::vector<int>& vA, const std::vector<int>& vB, int M, int N, int W)
{
	array_view<const int, 2> a(M, W, vA);
	array_view<const int, 2> b(W, N, vB);
	array_view<int, 2> c(M, N, vC);
	c.discard_data();	// do not transfer to GPU

	parallel_for_each(c.extent.tile<TS, TS>(), [=](tiled_index<TS, TS> t_idx) restrict(amp)
	{
		int row = t_idx.local[0];
		int col = t_idx.local[1];
		int sum = 0;

		tile_static int locA[TS][TS], locB[TS][TS];

		// copy to gpu static memory
		for (int i = 0; i < W; i += TS)
		{
			locA[row][col] = a(t_idx.global[0], col + i);
			locB[row][col] = b(row + i, t_idx.global[1]);
			t_idx.barrier.wait_with_tile_static_memory_fence();

			for (int k = 0; k < TS; k++)
				sum += locA[row][k] + locB[k][col];

			t_idx.barrier.wait_with_tile_static_memory_fence();
		}		
		
		c(t_idx.global) = sum;
	});

	c.synchronize(); // transfer back
}

///////////////////////////////////////////////////////////////////////////////
void TestAmpTiling()
{
	// M-by-W times W-by-N = M-by-N
	int M = 2;
	int W = 4;
	int N = 6;

	// 3 vectors represent matrix objects
	std::vector<int> A(M*W);
	std::vector<int> B(W*N);
	std::vector<int> C(M*N);

	// populate matrix A
	for (unsigned i = 0; i < A.size(); i++)
		A[i] = i + 1;

	// populate matrix B
	for (unsigned i = 0; i < B.size(); i++)
		B[i] = i + 1;

	// perform matrix multiplication computation
	//MatMul_Serial(C, A, B, M, N, W);
	//MatMul_AMP(C, A, B, M, N, W);
	MatMul_AMP_tiled(C, A, B, M, N, W);
}

///////////////////////////////////////////////////////////////////////////////
