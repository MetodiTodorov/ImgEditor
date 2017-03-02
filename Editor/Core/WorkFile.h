#pragma once
///////////////////////////////////////////////////////////////////////////////
#include <thread>
#include "History.h"
#include "Core/Timer.h"
#include "Task/ImgTask.h"

///////////////////////////////////////////////////////////////////////////////
class WorkFile
{
public:
	WorkFile();
	WorkFile(const wxString& path, const wxImage& img);
	WorkFile(WorkFile&& rhs);
	WorkFile& operator= (WorkFile&& rhs);

	void SaveAs(const wxString& path);

	const wxString& GetPath() const;
	const wxImage&	GetImage() const;
	wxImage&		GetImage();
	void			SetImage(const wxImage& img);

	// effects
	void			Blur(int horizontal, int vertical);
	void			Inverse();

	bool			IsChanged() const;
	void			CancelCurrentTask();

private:
	wxString		path_;
	wxImage			image_;	
	History			history_;

	bool			changed_;
};

///////////////////////////////////////////////////////////////////////////////