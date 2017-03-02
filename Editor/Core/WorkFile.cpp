#include <stdafx.h>
#include "WorkFile.h"
#include "Task/ImgTask.h"
#include "Task/BlurTask.h"
#include "Task/InverseTask.h"

///////////////////////////////////////////////////////////////////////////////
WorkFile::WorkFile()
	: changed_(false)
	, history_(*this)
{
}

///////////////////////////////////////////////////////////////////////////////
WorkFile::WorkFile(const wxString& path, const wxImage& img)
	: changed_(false)
	, history_(*this)
{
	path_ = path;
	image_ = img;
}

///////////////////////////////////////////////////////////////////////////////
WorkFile::WorkFile(WorkFile&& rhs)
	: path_(std::move(rhs.path_))
	, image_(std::move(rhs.image_))
	, history_(*this)
	, changed_(rhs.changed_)
{
}

///////////////////////////////////////////////////////////////////////////////
WorkFile& WorkFile::operator= (WorkFile&& rhs)
{
	path_ = std::move(rhs.path_);
	image_ = std::move(rhs.image_);
	history_ = std::move(rhs.history_);
	changed_ = rhs.changed_;

	return *this;
}

///////////////////////////////////////////////////////////////////////////////
void WorkFile::SaveAs(const wxString& path)
{
	if (path != path_)
		path_ = path;	

	auto imgType = PathToBmpType(path_);
	if (imgType)
	{
		image_.SaveFile(path_, imgType);
	}
}

///////////////////////////////////////////////////////////////////////////////
const wxString& WorkFile::GetPath() const
{
	return path_;
}

///////////////////////////////////////////////////////////////////////////////
const wxImage& WorkFile::GetImage() const
{
	return image_;
}

///////////////////////////////////////////////////////////////////////////////
wxImage& WorkFile::GetImage()
{
	return image_;
}

///////////////////////////////////////////////////////////////////////////////
void WorkFile::SetImage(const wxImage& img)
{
	image_ = img;
	changed_ = true;
}

///////////////////////////////////////////////////////////////////////////////
void WorkFile::Blur(int horizontal, int vertical)
{
	auto current = history_.GetCurrentTask();
	if (current && current->Is("Blur"))
	{
		auto blur = static_cast<BlurTask*>(current);
		if (!blur->HasParams(horizontal, vertical))
		{
			// blur with new params
			blur->SetParams(horizontal, vertical);
		}
	}
	else
	{
		auto prev = history_.GetPrevTask();
		if (prev && prev->Is("Blur"))
		{
			// our previous task was blur -> use its source image
			auto blur = new BlurTask(prev->GetSrcImage(), horizontal, vertical);
			history_.PushNewTask(blur);
		}
		else
		{
			// blur our screen image
			auto blur = new BlurTask(image_.Copy(), horizontal, vertical);
			history_.PushNewTask(blur);
		}		
	}
}

///////////////////////////////////////////////////////////////////////////////
void WorkFile::Inverse()
{
	auto current = history_.GetCurrentTask();
	if (current && current->Is("Inverse"))
	{
		// do nothing
	}
	else
	{
		// inverse image
		auto task = new InverseTask(image_.Copy());
		history_.PushNewTask(task);
	}
}

///////////////////////////////////////////////////////////////////////////////
bool WorkFile::IsChanged() const
{
	return changed_;
}

///////////////////////////////////////////////////////////////////////////////
void WorkFile::CancelCurrentTask()
{
	history_.CancelCurrentTask();
}

///////////////////////////////////////////////////////////////////////////////