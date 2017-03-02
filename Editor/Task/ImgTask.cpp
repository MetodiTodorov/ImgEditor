#include <stdafx.h>
#include "ImgTask.h"

///////////////////////////////////////////////////////////////////////////////
ImgTask::ImgTask(wxImage img, int steps)
	: src_(img)
	, dst_(img.GetWidth(), img.GetHeight())
	, regionSz_(20)
	, regionsX_((img.GetWidth() / regionSz_) + 1)
	, regionsY_((img.GetHeight() / regionSz_) + 1)
	, steps_(steps)
	, restarted_(false)
	, canceled_(false)
{
	if (steps_ <= 0)
		steps_ = img.GetWidth() * img.GetHeight();	
}

///////////////////////////////////////////////////////////////////////////////
const wxImage& ImgTask::GetSrcImage() const
{
	return src_;
}

///////////////////////////////////////////////////////////////////////////////
const wxImage& ImgTask::GetDstImage() const
{
	return dst_;
}

///////////////////////////////////////////////////////////////////////////////
int ImgTask::GetWidth() const
{
	return dst_.GetWidth();
}

///////////////////////////////////////////////////////////////////////////////
int ImgTask::GetHeight() const
{
	return dst_.GetHeight();
}

///////////////////////////////////////////////////////////////////////////////
std::string ImgTask::GetName() const
{
	return "ImgTask";
}

///////////////////////////////////////////////////////////////////////////////
bool ImgTask::Is(const std::string& name) const
{
	std::lock_guard<std::mutex> lk(mutex_);
	return GetName() == name;
}

///////////////////////////////////////////////////////////////////////////////
void ImgTask::Run()
{
	int step = 0;

	for (; step < steps_; step++)
	{
		Step(step);

		std::lock_guard<std::mutex> lk(mutex_);
		percent_ = step / float(steps_);
		if (restarted_)
		{
			restarted_ = false;
			step = -1;
		}

		BreakIf(canceled_);
	}

	if (step == steps_)
	{
		SetPercent(1.0f);
	}
}

///////////////////////////////////////////////////////////////////////////////
void ImgTask::Step(int step)
{
}

///////////////////////////////////////////////////////////////////////////////
void ImgTask::Restart()
{
	std::lock_guard<std::mutex> lk(mutex_);
	restarted_ = true;
}

///////////////////////////////////////////////////////////////////////////////
bool ImgTask::IsRestarted() const
{
	bool result = false;
	{
		std::lock_guard<std::mutex> lk(mutex_);
		result = restarted_;
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////////
void ImgTask::Cancel()
{
	std::lock_guard<std::mutex> lk(mutex_);
	canceled_ = true;
}

///////////////////////////////////////////////////////////////////////////////
bool ImgTask::IsCanceled() const
{
	bool result = false;
	{
		std::lock_guard<std::mutex> lk(mutex_);
		result = canceled_;
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////////
void ImgTask::Sync(wxImage& screenBuf)
{
	std::lock_guard<std::mutex> lk(mutex_);
	int width = dst_.GetWidth();
	int height = dst_.GetHeight();

	for (auto region : dirtyRegions_)
	{
		int regionX = region % regionsX_;
		int regionY = region / regionsX_;
		int begX = regionX * regionSz_;
		int endX = (regionX + 1) * regionSz_;
		int begY = regionY * regionSz_;
		int endY = (regionY + 1) * regionSz_;

		for (int x = begX; x < endX; x++)
		{
			ContinueIf(x >= width);
			for (int y = begY; y < endY; y++)
			{
				ContinueIf(y >= height);

				auto red = dst_.GetRed(x, y);
				auto green = dst_.GetGreen(x, y);
				auto blue = dst_.GetBlue(x, y);
				screenBuf.SetRGB(x, y, red, green, blue);

				if (dst_.HasAlpha())
				{
					screenBuf.SetAlpha(x, y, dst_.GetAlpha(x, y));
				}
			}
		}
	}

	dirtyRegions_.clear();
}

///////////////////////////////////////////////////////////////////////////////
void ImgTask::SetColor(int x, int y, const Color& c)
{
	// calculate dirty region
	int xRegion = x / regionSz_;
	int yRegion = y / regionSz_;
	int region = yRegion * regionsX_ + xRegion;

	// set color and alpha
	std::lock_guard<std::mutex> lk(mutex_);
	dst_.SetRGB(x, y, c.GetRedByte(), c.GetGreenByte(), c.GetBlueByte());

	if (src_.HasAlpha())
	{
		dst_.SetAlpha(x, y, c.GetAlphaByte());
	}

	// add this dirty region
	if (!Contains(dirtyRegions_, region))
	{
		dirtyRegions_.push_back(region);
	}
}

///////////////////////////////////////////////////////////////////////////////
std::vector<Color> ImgTask::GetNeighbors(int x, int y, int hRadius, int vRadius)
{
	std::lock_guard<std::mutex> lk(mutex_);
	std::vector<Color> result;

	for (int dx = x - hRadius; dx <= x + hRadius; dx++)
	{
		ContinueUnless(dx >= 0 && dx < src_.GetWidth());

		for (int dy = y - vRadius; dy <= y + vRadius; dy++)
		{
			ContinueUnless(dy >= 0 && dy < src_.GetHeight());
			
			Color color;
			color.SetRedByte(src_.GetRed(dx, dy));
			color.SetGreenByte(src_.GetGreen(dx, dy));
			color.SetBlueByte(src_.GetBlue(dx, dy));
			if (src_.HasAlpha())
			{
				color.SetAlphaByte(src_.GetAlpha(dx, dy));
			}
			
			result.push_back(color);
		}
	}
	
	return result;
}

///////////////////////////////////////////////////////////////////////////////