#include <stdafx.h>
#include "Commander.h"

///////////////////////////////////////////////////////////////////////////////
Commander::Commander()
{
	auto dispatcher = GetMsgDispatcher();
	dispatcher->Register(this);
}

///////////////////////////////////////////////////////////////////////////////
Commander::~Commander()
{
	auto dispatcher = GetMsgDispatcher();
	dispatcher->Unregister(this);
}

///////////////////////////////////////////////////////////////////////////////
void Commander::OnMsgReceived(const Message& msg)
{
	switch (msg.GetType())
	{
	DISPATCH(Save);
	DISPATCH(EndOpenFile);
	DISPATCH(BeginCloseFile);
	DISPATCH(EndSaveAs);
	DISPATCH(BeginBlurEffect);
	DISPATCH(BeginInverseEffect);
	}
}

///////////////////////////////////////////////////////////////////////////////
void Commander::OnSave(const Message&)
{
	auto fileMgr = GetFileMgr();
	fileMgr->SaveCurrent();
}

///////////////////////////////////////////////////////////////////////////////
void Commander::OnEndOpenFile(const Message& msg)
{
	auto openFileMsg = static_cast<const EndOpenFileMsg*>(&msg);
	auto& path = openFileMsg->Path;

	auto imgType = PathToBmpType(path);
	if (imgType)
	{
		wxBitmap bmp;
		if (bmp.LoadFile(path, imgType))
		{
			//WorkFile file(path, bmp.ConvertToImage());

			auto fileMgr = GetFileMgr();
			fileMgr->AddFile(path, bmp.ConvertToImage());

			auto dispatcher = GetMsgDispatcher();
			dispatcher->Raise(LoadFileMsg(path));
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void Commander::OnBeginCloseFile(const Message& msg)
{
	auto fileMgr = GetFileMgr();
	fileMgr->CloseCurrent();

	auto dispatcher = GetMsgDispatcher();
	dispatcher->Raise(Message::EndCloseFile);
}

///////////////////////////////////////////////////////////////////////////////
void Commander::OnEndSaveAs(const Message& msg)
{		
	auto newTitleMsg = static_cast<const EndSaveAsMsg*>(&msg);
	auto fileMgr = GetFileMgr();

	fileMgr->SaveCurrentAs(newTitleMsg->Path);
}

///////////////////////////////////////////////////////////////////////////////
void Commander::OnBeginBlurEffect(const Message& msg)
{
	auto fileMgr = GetFileMgr();
	auto current = fileMgr->GetCurrent();
	AssertReturnUnless(current);

	auto& blurMsg = static_cast<const BeginBlurMsg&>(msg);
	current->Blur(blurMsg.Horizontal, blurMsg.Vertical);
}

///////////////////////////////////////////////////////////////////////////////
void Commander::OnBeginInverseEffect(const Message& msg)
{
	auto fileMgr = GetFileMgr();
	auto current = fileMgr->GetCurrent();
	AssertReturnUnless(current);

	current->Inverse();
}

///////////////////////////////////////////////////////////////////////////////