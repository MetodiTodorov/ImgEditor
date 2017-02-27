#include <stdafx.h>
#include "Utils.h"

///////////////////////////////////////////////////////////////////////////////
wxBitmapType PathToBmpType(const wxString& path)
{
	if (path.EndsWith(".bmp") || path.EndsWith(".bmp"))
		return wxBITMAP_TYPE_BMP;
	if (path.EndsWith(".jpeg") || path.EndsWith(".jpg"))
		return wxBITMAP_TYPE_JPEG;
	if (path.EndsWith(".gif"))
		return wxBITMAP_TYPE_GIF;
	if (path.EndsWith(".tiff") || path.EndsWith(".tif"))
		return wxBITMAP_TYPE_TIFF;
	if (path.EndsWith(".png"))
		return wxBITMAP_TYPE_PNG;

	return wxBITMAP_TYPE_INVALID;
}

///////////////////////////////////////////////////////////////////////////////