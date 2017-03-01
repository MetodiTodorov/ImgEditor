#pragma once

#include <wx/wx.h>

///////////////////////////////////////////////////////////////////////////////
class Timer : public wxTimer
{
public:
	Timer();
	Timer(std::function<void()> fn);
	Timer(const Timer& rhs);
	Timer& operator= (const Timer& rhs);
	virtual ~Timer() = default;	

	virtual void Notify() override;
	void SetCallback(std::function<void()> fn);

private:
	std::function<void()>	callback_;
};

///////////////////////////////////////////////////////////////////////////////
