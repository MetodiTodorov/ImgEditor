#include <stdafx.h>
#include "MainFrame.h"
#include "Monitor.h"
#include "MainMenu.h"
#include "StatusBar.h"
#include "ControlId.h"
#include "BlurPane.h"
#include "Msg/MsgDispatcher.h"
#include "Utils/Defensive.h"
#include "Core/Commander.h"
#include "Core/FileMgr.h"

///////////////////////////////////////////////////////////////////////////////
#include "Test/Test.h"

///////////////////////////////////////////////////////////////////////////////
MainFrame::MainFrame(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(640, 480))
{
	mainPanel_ = new wxPanel(this);

	sizerMain_ = new wxBoxSizer(wxHORIZONTAL);
	sizerLeft_ = new wxBoxSizer(wxVERTICAL);
	sizerRight_ = new wxBoxSizer(wxVERTICAL);
	sizerMain_->Add(sizerLeft_, 0, wxSTRETCH_NOT);
	sizerMain_->Add(sizerRight_, 1, wxEXPAND);

	blurPane_ = new BlurPane(mainPanel_);
	sizerLeft_->Add(blurPane_->GetConnector());

	monitor_ = new Monitor(mainPanel_);
	sizerRight_->Add(monitor_, 1, wxEXPAND);
	
	mainPanel_->SetSizer(sizerMain_);
	SetMenuBar(new MainMenu);
	SetStatusBar(new StatusBar(this));

	// events
	//Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MainFrame::OnClose));
	Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);

	auto dispatcher = GetMsgDispatcher();
	dispatcher->Register(this, Message::BeginOpenFile);
	dispatcher->Register(this, Message::BeginExit);
	dispatcher->Register(this, Message::LoadFile);
	dispatcher->Register(this, Message::BeginSaveAs);
	dispatcher->Register(this, Message::EndCloseFile);

	Show();

	// start updating
	timer_.SetCallback(std::bind(&Monitor::Update, monitor_));
	timer_.Start(20);


	TestAmp();
	TestAmpTiling();
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::OnClose(wxCloseEvent& ev)
{
	ev.Veto();

	auto dispatcher = GetMsgDispatcher();
	dispatcher->Raise(Message::BeginExit);
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::OnMsgReceived(const Message& msg)
{
	switch (msg.GetType())
	{
		DISPATCH(BeginOpenFile);
		DISPATCH(BeginExit);
		DISPATCH(LoadFile);
		DISPATCH(BeginSaveAs);
		DISPATCH(EndCloseFile);
	}
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::OnBeginOpenFile(const Message& msg)
{
	wxString wildcard;
	wildcard.Append("All Picture Files|*.bmp;*.dib;*.jpeg;*.jpg;*.gif;*.tiff;*.tif;*.png|");
	wildcard.Append("Bitmap Files (*.bmp;*.dib)|*.bmp;*.dib|");
	wildcard.Append("JPEG (*.jpeg;*.jpg)|*.jpeg;*.jpg|");
	//wildcard.Append("GIF (*.gif)|*.gif|");	
	wildcard.Append("TIFF (*.tiff;*.tif)|*.tiff;*.tif|");
	wildcard.Append("PNG (*.png)|*.png");

	wxFileDialog dlg(this, wxT("Open Image file"), "", "", wildcard, 
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (dlg.ShowModal() == wxID_OK)
	{
		auto dispatcher = GetMsgDispatcher();
		dispatcher->Raise(EndOpenFileMsg(dlg.GetPath()));
	}
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::OnBeginExit(const Message& msg)
{
	auto fileMgr = GetFileMgr();
	auto current = fileMgr->GetCurrent();
	if (!current || !current->IsChanged())
	{
		Destroy();
		return;
	}

	wxMessageDialog dlg(this, wxT("Are you sure to quit?"),	wxT("Question"),
		wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

	if (dlg.ShowModal() == wxID_YES)
	{
		if (monitor_)
		{
			monitor_->SetWorkFile(nullptr);
		}
		
		Destroy();
	}
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::OnLoadFile(const Message& msg)
{
	auto fileMgr = GetFileMgr();
	auto loadMsg = static_cast<const LoadFileMsg*>(&msg);
	auto* workFile = fileMgr->GetFile(loadMsg->Path);
	AssertReturnUnless(workFile);

	monitor_->SetWorkFile(workFile);
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::OnBeginSaveAs(const Message& msg)
{
	auto fileMgr = GetFileMgr();
	auto current = fileMgr->GetCurrent();
	AssertReturnUnless(current);

	wxString wildcard;
	wildcard.Append("All Picture Files|*.bmp;*.dib;*.jpeg;*.jpg;*.gif;*.tiff;*.tif;*.png|");
	wildcard.Append("Bitmap Files (*.bmp;*.dib)|*.bmp;*.dib|");
	wildcard.Append("JPEG (*.jpeg;*.jpg)|*.jpeg;*.jpg|");
	//wildcard.Append("GIF (*.gif)|*.gif|");	
	wildcard.Append("TIFF (*.tiff;*.tif)|*.tiff;*.tif|");
	wildcard.Append("PNG (*.png)|*.png");
	
	wxFileDialog dlg(this, _("Save Image file"), "", current->GetPath(),
			wildcard, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (dlg.ShowModal() == wxID_OK)
	{
		auto dispatcher = GetMsgDispatcher();
		dispatcher->Raise(EndSaveAsMsg(dlg.GetPath()));
	}
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::OnEndCloseFile(const Message& msg)
{
	auto fileMgr = GetFileMgr();
	auto current = fileMgr->GetCurrent();
	monitor_->SetWorkFile(current);
}

///////////////////////////////////////////////////////////////////////////////