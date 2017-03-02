#include <stdafx.h>
#include "MainMenu.h"
#include "ControlId.h"
#include "Msg/MsgDispatcher.h"
#include "Core/FileMgr.h"

///////////////////////////////////////////////////////////////////////////////
MainMenu::MainMenu()
{
	CreateFileMenu();
	CreateEffectMenu();

	auto dispatcher = GetMsgDispatcher();
	dispatcher->Register(this, Message::LoadFile);
	dispatcher->Register(this, Message::EndCloseFile);
}

///////////////////////////////////////////////////////////////////////////////
void MainMenu::CreateFileMenu()
{	
	file_ = new wxMenu;
	save_ = new wxMenuItem(file_, wxID_SAVE, wxT("&Save\tCtrl+S"));
	saveAs_ = new wxMenuItem(file_, wxID_SAVEAS, wxT("Save As"));
	closeFile_ = new wxMenuItem(file_, wxID_CLOSE, wxT("Close"));
	
	file_->Append(wxID_OPEN, wxT("&Open\tCtrl+O"));
	file_->Append(save_);
	file_->Append(saveAs_);
	file_->Append(closeFile_);
	file_->AppendSeparator();
	file_->Append(wxID_EXIT, wxT("&Quit\tAlt+F4"));
	Append(file_, wxT("&File"));
	
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainMenu::OnOpen, this, wxID_OPEN);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainMenu::OnSave, this, wxID_SAVE);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainMenu::OnSaveAs, this, wxID_SAVEAS);	
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainMenu::OnClose, this, wxID_CLOSE);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainMenu::OnQuit, this, wxID_EXIT);

	RefreshFileMenus();
}

///////////////////////////////////////////////////////////////////////////////
void MainMenu::CreateEffectMenu()
{
	effect_ = new wxMenu;
	blur_ = new wxMenuItem(effect_, ID_MM_Blur, wxT("Blur"));
	inverse_ = new wxMenuItem(effect_, ID_MM_Inverse, wxT("Inverse"));

	effect_->Append(blur_);
	effect_->Append(inverse_);

	Append(effect_, wxT("&Effect"));

	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainMenu::OnBlur, this, ID_MM_Blur);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainMenu::OnInverse, this, ID_MM_Inverse);

	RefreshEffectMenus();
}

///////////////////////////////////////////////////////////////////////////////
void MainMenu::OnSave(wxCommandEvent& ev)
{
	auto dispatcher = GetMsgDispatcher();
	dispatcher->Raise(Message::Save);	
}

///////////////////////////////////////////////////////////////////////////////
void MainMenu::OnSaveAs(wxCommandEvent& ev)
{
	auto dispatcher = GetMsgDispatcher();
	dispatcher->Raise(Message::BeginSaveAs);
}

///////////////////////////////////////////////////////////////////////////////
void MainMenu::OnClose(wxCommandEvent& ev)
{
	auto dispatcher = GetMsgDispatcher();
	dispatcher->Raise(Message::BeginCloseFile);
}

///////////////////////////////////////////////////////////////////////////////
void MainMenu::OnOpen(wxCommandEvent& ev)
{
	auto dispatcher = GetMsgDispatcher();
	dispatcher->Raise(Message::BeginOpenFile);
}

///////////////////////////////////////////////////////////////////////////////
void MainMenu::OnQuit(wxCommandEvent& ev)
{
	auto dispatcher = GetMsgDispatcher();
	dispatcher->Raise(Message::BeginExit);
}

///////////////////////////////////////////////////////////////////////////////
void MainMenu::OnBlur(wxCommandEvent& ev)
{
	auto dispatcher = GetMsgDispatcher();
	dispatcher->Raise(Message::BeginBlurEffect);
}

///////////////////////////////////////////////////////////////////////////////
void MainMenu::OnInverse(wxCommandEvent& ev)
{
	auto dispatcher = GetMsgDispatcher();
	dispatcher->Raise(Message::BeginInverseEffect);
}

///////////////////////////////////////////////////////////////////////////////
void MainMenu::OnMsgReceived(const Message& msg)
{
	switch (msg.GetType())
	{
		DISPATCH(LoadFile);
		DISPATCH(EndCloseFile);
	}
}

///////////////////////////////////////////////////////////////////////////////
void MainMenu::OnLoadFile(const Message& msg)
{
	RefreshFileMenus();
	RefreshEffectMenus();
}

///////////////////////////////////////////////////////////////////////////////
void MainMenu::OnEndCloseFile(const Message& msg)
{
	RefreshFileMenus();
	RefreshEffectMenus();
}

///////////////////////////////////////////////////////////////////////////////
void MainMenu::RefreshFileMenus()
{
	auto fileMgr = GetFileMgr();
	auto enableSave = fileMgr->GetCurrent() != nullptr;

	save_->Enable(enableSave);
	saveAs_->Enable(enableSave);
	closeFile_->Enable(enableSave);
}

///////////////////////////////////////////////////////////////////////////////
void MainMenu::RefreshEffectMenus()
{
	auto fileMgr = GetFileMgr();
	auto enableSave = fileMgr->GetCurrent() != nullptr;

	blur_->Enable(enableSave);
}

///////////////////////////////////////////////////////////////////////////////