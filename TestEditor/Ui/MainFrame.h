#pragma once

#include <wx/wx.h>
#include "Msg/MsgReceiver.h"
#include "Core/Timer.h"

///////////////////////////////////////////////////////////////////////////////
class Monitor;
class BlurPane;

///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame, public MsgReceiver
{
public:
	MainFrame(const wxString& title);

public:
	void OnClose(wxCloseEvent& ev);

	void OnMsgReceived(const Message& msg) override;
	void OnBeginOpenFile(const Message& msg);
	void OnBeginExit(const Message& msg);
	void OnLoadFile(const Message& msg);
	void OnBeginSaveAs(const Message& msg);
	void OnEndCloseFile(const Message& msg);

private:
	Timer		timer_;
	Monitor*	monitor_;

	BlurPane*	blurPane_;

	wxPanel*	mainPanel_;
	wxBoxSizer* sizerMain_;
	wxBoxSizer* sizerLeft_;
	wxBoxSizer* sizerRight_;
};

///////////////////////////////////////////////////////////////////////////////