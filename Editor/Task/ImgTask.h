#pragma once

///////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <wx/wx.h>
#include "Core/Color.h"
#include "Task.h"

///////////////////////////////////////////////////////////////////////////////
class ImgTask : public Task
{
public:
	ImgTask(wxImage img, int steps = -1);
	virtual ~ImgTask() = default;

	const wxImage&		GetSrcImage() const;
	const wxImage&		GetDstImage() const;

	int					GetWidth() const;
	int					GetHeight() const;	
	virtual std::string	GetName() const;
	virtual bool		Is(const std::string& name) const;

	virtual void		Run() override;
	virtual void		Step(int step);

	void				Restart();
	bool				IsRestarted() const;
	void				Cancel();
	bool				IsCanceled() const;
	void				Sync(wxImage& screenBuf);

protected:
	void SetColor(int x, int y, const Color& c);
	std::vector<Color> GetNeighbors(int x, int y, int hRadius = 1, int vRadius = 1);

private:	
	std::vector<int>	dirtyRegions_;
	const int			regionSz_;
	const int			regionsX_;
	const int			regionsY_;

protected:
	const wxImage		src_;	
	wxImage				dst_;
	int					steps_;
	bool				restarted_;
	bool				canceled_;
};

///////////////////////////////////////////////////////////////////////////////