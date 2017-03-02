#pragma once

#include <wx/wx.h>
///////////////////////////////////////////////////////////////////////////////
#define DISPATCH(MsgType) case Message::##MsgType: On##MsgType(msg); break
///////////////////////////////////////////////////////////////////////////////
class Message
{
public:
	enum Type
	{
		BeginExit,			// wanna exit
		BeginOpenFile,		// wanna open file
		EndOpenFile,		// open filepath
		BeginCloseFile,		// wanna close current work file
		EndCloseFile,		// current file was closed
		LoadFile,			// file was loaded		
		Save,				// save current work file
		BeginSaveAs,		// wanna save with new title
		EndSaveAs,			// save pic with new title
		BeginBlurEffect,	// start blur effect
		BeginInverseEffect,	// start inverse effect
		Working,			// working status
	};

public:
	Message(Type type)
		: type_(type) 
	{
	}

	Type GetType() const
	{
		return type_;
	}

private:
	const Type type_;
};

///////////////////////////////////////////////////////////////////////////////
struct EndOpenFileMsg : public Message
{
	EndOpenFileMsg(const wxString& path)
		: Message(Message::EndOpenFile)
		, Path(path)
	{}

	wxString Path;
};

///////////////////////////////////////////////////////////////////////////////
struct EndSaveAsMsg : public Message
{
	EndSaveAsMsg(const wxString& path)
		: Message(Message::EndSaveAs)
		, Path(path)
	{}

	wxString Path;
};

///////////////////////////////////////////////////////////////////////////////
struct LoadFileMsg : public Message
{
	LoadFileMsg(const wxString& path)
		: Message(Message::LoadFile)
		, Path(path)
	{}

	wxString Path;
};

///////////////////////////////////////////////////////////////////////////////
struct BeginBlurMsg : public Message
{
	BeginBlurMsg(int hor, int vert)
		: Message(Message::BeginBlurEffect)
		, Horizontal(hor)
		, Vertical(vert)
	{}

	int Horizontal, Vertical;
};

///////////////////////////////////////////////////////////////////////////////
struct WorkingMsg : public Message
{
	WorkingMsg(wxString msg, float percent)
		: Message(Message::Working)
		, Text(std::move(msg))
		, Percent(percent)
	{
	}

	wxString Text;
	float Percent;
};

///////////////////////////////////////////////////////////////////////////////