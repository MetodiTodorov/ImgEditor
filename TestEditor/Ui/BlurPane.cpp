#include <stdafx.h>
#include "BlurPane.h"
#include "ControlId.h"
#include "MainFrame.h"
#include "Msg/Message.h"

///////////////////////////////////////////////////////////////////////////////
BlurPane::BlurPane(wxPanel* parent)
{
	sizer_ = new wxStaticBoxSizer(wxVERTICAL, parent, wxT("Blur"));
	sliderH_ = new wxSlider(parent, ID_SLD_BlurH, 0, 0, 10);
	sliderV_ = new wxSlider(parent, ID_SLD_BlurV, 0, 0, 10);
	textH_ = new wxStaticText(parent, wxID_ANY, wxT("0"));
	textV_ = new wxStaticText(parent, wxID_ANY, wxT("0"));	

	auto row1 = new wxBoxSizer(wxHORIZONTAL);
	row1->Add(new wxStaticText(parent, wxID_ANY, wxT("Hor: ")), 2);	
	row1->Add(sliderH_, 6);	
	row1->Add(textH_, 1);
	sizer_->Add(row1);
	
	auto row2 = new wxBoxSizer(wxHORIZONTAL);
	row2->Add(new wxStaticText(parent, wxID_ANY, wxT("Ver: ")), 2);
	row2->Add(sliderV_, 6);
	row2->Add(textV_, 1);
	sizer_->Add(row2);

	textH_->Enable(false);
	textV_->Enable(false);
	sliderH_->Enable(false);
	sliderV_->Enable(false);

	sliderH_->Bind(wxEVT_COMMAND_SLIDER_UPDATED, &BlurPane::OnHSlide, this, ID_SLD_BlurH);
	sliderV_->Bind(wxEVT_COMMAND_SLIDER_UPDATED, &BlurPane::OnVSlide, this, ID_SLD_BlurV);
	
	auto dispatcher = GetMsgDispatcher();
	dispatcher->Register(this, Message::LoadFile);
	dispatcher->Register(this, Message::EndCloseFile);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void BlurPane::OnMsgReceived(const Message& msg)
{
	switch (msg.GetType())
	{
		DISPATCH(LoadFile);
		DISPATCH(EndCloseFile);
	}
}

///////////////////////////////////////////////////////////////////////////////
void BlurPane::OnLoadFile(const Message& msg)
{
	textH_->SetLabelText(wxT("0"));
	textV_->SetLabelText(wxT("0"));
	sliderH_->SetValue(0);
	sliderV_->SetValue(0);

	textH_->Enable(true);
	textV_->Enable(true);
	sliderH_->Enable(true);
	sliderV_->Enable(true);
}

///////////////////////////////////////////////////////////////////////////////
void BlurPane::OnEndCloseFile(const Message& msg)
{
	textH_->Enable(false);
	textV_->Enable(false);
	sliderH_->Enable(false);
	sliderV_->Enable(false);
}

///////////////////////////////////////////////////////////////////////////////
wxStaticBoxSizer* BlurPane::GetConnector()
{
	return sizer_;
}

///////////////////////////////////////////////////////////////////////////////
void BlurPane::OnHSlide(wxCommandEvent& ev)
{
	int horizontal = sliderH_->GetValue();
	int vertical = sliderV_->GetValue();

	auto text = wxString::Format("%d", horizontal);
	textH_->SetLabelText(text);

	auto dispatcher = GetMsgDispatcher();
	dispatcher->Raise(BeginBlurMsg(horizontal, vertical));
}

///////////////////////////////////////////////////////////////////////////////
void BlurPane::OnVSlide(wxCommandEvent& ev)
{
	int horizontal = sliderH_->GetValue();
	int vertical = sliderV_->GetValue();

	auto text = wxString::Format("%d", vertical);
	textV_->SetLabelText(text);

	auto dispatcher = GetMsgDispatcher();
	dispatcher->Raise(BeginBlurMsg(horizontal, vertical));
}

///////////////////////////////////////////////////////////////////////////////