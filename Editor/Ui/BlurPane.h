#pragma once

///////////////////////////////////////////////////////////////////////////////
#include <wx/wx.h>

class MainFrame;

///////////////////////////////////////////////////////////////////////////////
class BlurPane : public MsgReceiver
{
public:
	BlurPane(wxPanel* mainPanel);

	wxStaticBoxSizer*	GetConnector();

private:
	void				OnMsgReceived(const Message& msg) override;
	void				OnLoadFile(const Message& msg);
	void				OnEndCloseFile(const Message& msg);

	void				OnHSlide(wxCommandEvent& ev);
	void				OnVSlide(wxCommandEvent& ev);

public:
	wxStaticBoxSizer*	sizer_;

	wxStaticText*		textH_;
	wxStaticText*		textV_;
	wxSlider*			sliderH_;	
	wxSlider*			sliderV_;	
};

///////////////////////////////////////////////////////////////////////////////