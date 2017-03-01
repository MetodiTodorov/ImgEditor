#include <stdafx.h>
#include "StatusBar.h"
#include "Msg/MsgDispatcher.h"

///////////////////////////////////////////////////////////////////////////////
StatusBar::StatusBar(wxFrame* parent)
	: wxStatusBar(parent, wxID_ANY)
	, MsgReceiver(VERY_LOW)
{
	auto dispatcher = GetMsgDispatcher();
	dispatcher->Register(this);
}

///////////////////////////////////////////////////////////////////////////////
StatusBar::~StatusBar()
{
	auto dispatcher = GetMsgDispatcher();
	dispatcher->Unregister(this);
}

///////////////////////////////////////////////////////////////////////////////
void StatusBar::OnMsgReceived(const Message& msg)
{
	switch (msg.GetType())
	{
	DISPATCH(LoadFile);
	DISPATCH(Working);
	}
}

///////////////////////////////////////////////////////////////////////////////
void StatusBar::OnLoadFile(const Message& msg)
{
	auto loadFileMsg = static_cast<const LoadFileMsg&>(msg);
	SetStatusText(wxString("Load File: ") + loadFileMsg.Path);
}

///////////////////////////////////////////////////////////////////////////////
void StatusBar::OnWorking(const Message& msg)
{
	auto workingMsg = static_cast<const WorkingMsg&>(msg);
	auto statusText = wxString::Format("%s: %.2f%%", workingMsg.Text.c_str(), workingMsg.Percent * 100.f);

	SetStatusText(statusText);
}

///////////////////////////////////////////////////////////////////////////////