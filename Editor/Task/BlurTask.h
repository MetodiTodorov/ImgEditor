#pragma once

///////////////////////////////////////////////////////////////////////////////
#include "ImgTask.h"

///////////////////////////////////////////////////////////////////////////////
class BlurTask : public ImgTask
{
public:
	BlurTask(wxImage img, int horizontal, int vertical);
	virtual ~BlurTask() = default;

	virtual void		Step(int step) override;
	virtual std::string	GetName() const override;

	void	SetParams(int horizontal, int vertical);
	bool	HasParams(int horizontal, int vertical) const;

	int		GetHorizontal() const;
	int		GetVertical() const;

private:
	int horizontal_;
	int vertical_;
};

///////////////////////////////////////////////////////////////////////////////