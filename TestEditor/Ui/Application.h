#pragma once

#include <wx/wx.h>

///////////////////////////////////////////////////////////////////////////////
class MainFrame;
class MsgDispatcher;
class Commander;
class FileMgr;

///////////////////////////////////////////////////////////////////////////////
class Application : public wxApp
{
public:
	virtual bool		OnInit() override;
	virtual int			OnExit() override;

	MsgDispatcher*		GetMsgDispatcher();
	Commander*			GetCommander();
	FileMgr*			GetFileMgr();

	static Application* Instance();

private: 
	MainFrame*			main_;
	MsgDispatcher*		dispatcher_;
	Commander*			commander_;
	FileMgr*			fileMgr_;
};

///////////////////////////////////////////////////////////////////////////////