#pragma once
///////////////////////////////////////////////////////////////////////////////
#include <wx/wx.h>
#include "WorkFile.h"
#include <map>
#include <string>

///////////////////////////////////////////////////////////////////////////////
class FileMgr
{
public:
	FileMgr();

	void			AddFile(const wxString& path, const wxImage& file);
	WorkFile*		GetCurrent();
	void			SaveCurrent();
	void			SaveCurrentAs(const wxString& path);
	void			CloseCurrent();

	WorkFile*		GetFile(const wxString& path);
	const WorkFile* GetFile(const wxString& path) const;

private:
	std::map<wxString, WorkFile>	files_;
	WorkFile*						current_;
};

///////////////////////////////////////////////////////////////////////////////
extern FileMgr* GetFileMgr();

///////////////////////////////////////////////////////////////////////////////