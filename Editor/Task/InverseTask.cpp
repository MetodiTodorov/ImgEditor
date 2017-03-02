#include <stdafx.h>
#include "InverseTask.h"

#include <windows.h>
#include <amp.h>

///////////////////////////////////////////////////////////////////////////////
InverseTask::InverseTask(wxImage img)
	: ImgTask(img, 1)
{
}

///////////////////////////////////////////////////////////////////////////////
void InverseTask::Step(int step)
{
	//Inverse();
	InverseAccelerated();
}

///////////////////////////////////////////////////////////////////////////////
std::string	InverseTask::GetName() const
{
	return "Inverse";
}

///////////////////////////////////////////////////////////////////////////////
void InverseTask::Inverse()
{
	auto rgb = src_.GetData();
	auto bytes = src_.GetWidth() * src_.GetHeight() * 3;
	auto rgbNew = new unsigned char[bytes];

	for (int i = 0; i < bytes; i++)
	{
		rgbNew[i] = 255 - rgb[i];
	}

	dst_.SetData(rgbNew);
}

///////////////////////////////////////////////////////////////////////////////
void InverseTask::InverseAccelerated()
{
	using namespace concurrency;
	auto colors = ImgToFloat4(src_);
	array_view<graphics::float_4> vColors(colors.size(), colors);

	parallel_for_each(vColors.extent, [=](index<1> idx) restrict(amp)
	{
		vColors[idx] = 1.0f - vColors[idx];
	});

	vColors.synchronize();

	Float4ToImg(colors, dst_);
}

///////////////////////////////////////////////////////////////////////////////