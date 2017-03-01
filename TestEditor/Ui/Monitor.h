#pragma once

///////////////////////////////////////////////////////////////////////////////
class Monitor : public wxPanel
{
public:
	Monitor(wxWindow* parent);
	virtual ~Monitor() = default;
	virtual void Update();
	virtual void Render(wxDC& dc);

	void		SetWorkFile(WorkFile* file);

private:
	WorkFile*	workFile_;
};

///////////////////////////////////////////////////////////////////////////////