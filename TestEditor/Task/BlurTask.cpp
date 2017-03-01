#include <stdafx.h>
#include "BlurTask.h"

///////////////////////////////////////////////////////////////////////////////
BlurTask::BlurTask(wxImage img, int horizontal, int vertical)
	: ImgTask(img)
	, horizontal_(horizontal)
	, vertical_(vertical)
{
}

///////////////////////////////////////////////////////////////////////////////
void BlurTask::Step(int step)
{
	int width = GetWidth();
	int y = step / width;
	int x = step % width;

	auto neighbors = GetNeighbors(x, y, horizontal_, vertical_);
	SetColor(x, y, Avg(neighbors));
}

///////////////////////////////////////////////////////////////////////////////
std::string	BlurTask::GetName() const
{
	return "Blur";
}

///////////////////////////////////////////////////////////////////////////////
void BlurTask::SetParams(int horizontal, int vertical)
{
	std::unique_lock<std::mutex> lk(mutex_);
	horizontal_ = horizontal;
	vertical_ = vertical;
	restarted_ = true;
}

///////////////////////////////////////////////////////////////////////////////
bool BlurTask::HasParams(int horizontal, int vertical) const
{
	std::unique_lock<std::mutex> lk(mutex_);
	return horizontal_ == horizontal && vertical_ == vertical;
}

///////////////////////////////////////////////////////////////////////////////
int BlurTask::GetHorizontal() const
{
	return horizontal_;
}

///////////////////////////////////////////////////////////////////////////////
int BlurTask::GetVertical() const
{
	return vertical_;
}

///////////////////////////////////////////////////////////////////////////////