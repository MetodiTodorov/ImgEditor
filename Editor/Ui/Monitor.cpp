#include <stdafx.h>
#include "Monitor.h"
#include <wx/dcbuffer.h>

///////////////////////////////////////////////////////////////////////////////
Monitor::Monitor(wxWindow* parent)
	: wxPanel(parent)
	, workFile_(nullptr)
{	
}

///////////////////////////////////////////////////////////////////////////////
void Monitor::Update()
{
	wxClientDC cdc(this);
	wxBufferedDC bcd(&cdc, cdc.GetSize());
	//wxBufferedDC bcd(&cdc, *buffer_);
	Render(bcd);
}

///////////////////////////////////////////////////////////////////////////////
void Monitor::Render(wxDC& dc)
{
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();

	if (workFile_)
	{
		// Fit, Fill, 1:1

		// draw centered image
		auto areaSz = GetSize();
		auto& img = workFile_->GetImage();
		int imgX = (areaSz.GetWidth() - img.GetWidth()) / 2;
		int imgY = (areaSz.GetHeight() - img.GetHeight()) / 2;

		dc.DrawBitmap(wxBitmap(img), imgX, imgY);		
	}
}

///////////////////////////////////////////////////////////////////////////////
void Monitor::SetWorkFile(WorkFile* file)
{
	workFile_ = file;
}

///////////////////////////////////////////////////////////////////////////////