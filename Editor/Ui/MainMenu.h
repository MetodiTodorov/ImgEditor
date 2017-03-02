#pragma once

///////////////////////////////////////////////////////////////////////////////
#include <wx/wx.h>
#include <wx/menu.h>
#include "Msg/MsgReceiver.h"

///////////////////////////////////////////////////////////////////////////////
class MainMenu : public wxMenuBar, public MsgReceiver
{
public:
	MainMenu();

private:
	void CreateFileMenu();
	void CreateEffectMenu();
	void OnSave(wxCommandEvent& ev);
	void OnSaveAs(wxCommandEvent& ev);
	void OnClose(wxCommandEvent& ev);	
	void OnOpen(wxCommandEvent& ev);
	void OnQuit(wxCommandEvent& ev);
	void OnBlur(wxCommandEvent& ev);
	void OnInverse(wxCommandEvent& ev);	

	void OnMsgReceived(const Message& msg) override;
	void OnLoadFile(const Message& msg);	
	void OnEndCloseFile(const Message& msg);

	void RefreshFileMenus();
	void RefreshEffectMenus();

private:
	wxMenu*		file_;
	wxMenuItem*	save_;
	wxMenuItem*	saveAs_;
	wxMenuItem*	closeFile_;

	wxMenu*		effect_;
	wxMenuItem*	blur_;
	wxMenuItem*	inverse_;
};

///////////////////////////////////////////////////////////////////////////////