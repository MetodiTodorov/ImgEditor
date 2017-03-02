#include <stdafx.h>
#include "InverseTask.h"

///////////////////////////////////////////////////////////////////////////////
InverseTask::InverseTask(wxImage img)
	: ImgTask(img, 1)
{
}

///////////////////////////////////////////////////////////////////////////////
void InverseTask::Step(int step)
{
	auto data = src_.GetData();
	auto bytes = src_.GetWidth() * src_.GetHeight() * 3;
	auto newData = new unsigned char[bytes];

	for (int i = 0; i < bytes; i++)
	{
		newData[i] = 255 - data[i];
	}

	dst_.SetData(newData);
}

///////////////////////////////////////////////////////////////////////////////
std::string	InverseTask::GetName() const
{
	return "Inverse";
}

///////////////////////////////////////////////////////////////////////////////