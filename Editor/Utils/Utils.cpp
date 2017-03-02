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
std::vector<concurrency::graphics::float_4> ImgToFloat4(const wxImage& img)
{
	auto pixels = img.GetWidth() * img.GetHeight();
	auto rgb = img.GetData();
	auto alpha = img.GetAlpha();
	std::vector<concurrency::graphics::float_4> buff(pixels);

	for (int i = 0; i < pixels; i++)
	{
		buff[i].a = alpha ? alpha[i] / 255.0f : 1.0f;
		buff[i].r = rgb[i * 3 + 0] / 255.0f;
		buff[i].g = rgb[i * 3 + 1] / 255.0f;
		buff[i].b = rgb[i * 3 + 2] / 255.0f;
	}

	return buff;
}

/////////////////////////////////////////////////////////////////////////////////
void Float4ToImg(const std::vector<concurrency::graphics::float_4>& from, wxImage& to)
{
	auto pixels = from.size();
	auto rgb = (unsigned char*)malloc(pixels * 3);
	auto alpha = to.HasAlpha() ? (unsigned char*)malloc(pixels) : nullptr;

	for (unsigned i = 0; i < pixels; i++)
	{
		if (alpha)
			alpha[i] = static_cast<unsigned char>(from[i].a * 255.0f);

		rgb[i * 3 + 0] = static_cast<unsigned char>(from[i].r * 255.0f);
		rgb[i * 3 + 1] = static_cast<unsigned char>(from[i].g * 255.0f);
		rgb[i * 3 + 2] = static_cast<unsigned char>(from[i].b * 255.0f);
	}

	to.SetData(rgb);
	to.SetAlpha(alpha);	
}

///////////////////////////////////////////////////////////////////////////////
std::vector<unsigned> CopyToARGB(const wxImage& img)
{
	int pixels = img.GetWidth() * img.GetHeight();
	auto rgb = img.GetData();
	auto alpha = img.GetAlpha();
	std::vector<unsigned> buff;;

	for (int i = 0; i < pixels; i++)
	{
		SetAlpha(alpha ? alpha[i] : 255, buff[i]);
		SetRed(rgb[i*3], buff[i]);
		SetGreen(rgb[i*3 + 1], buff[i]);
		SetBlue(rgb[i*3 + 2], buff[i]);
	}

	return buff;
}

///////////////////////////////////////////////////////////////////////////////
void ARGBToImg(const std::vector<unsigned>& from, wxImage& to)
{
	auto rgb = new unsigned char[from.size() * 3];
	auto alpha = to.HasAlpha() ? new unsigned char[from.size()] : nullptr;

	for (unsigned i = 0; i < from.size(); i++)
	{
		if (alpha)
			alpha[i] = GetAlpha(from[i]);

		rgb[i*3 + 0] = GetRed(from[i]);
		rgb[i*3 + 1] = GetGreen(from[i]);
		rgb[i*3 + 2] = GetBlue(from[i]);
	}

	to.SetData(rgb);
	to.SetAlpha(alpha);
}

///////////////////////////////////////////////////////////////////////////////
unsigned char GetAlpha(unsigned argb)
{
	return static_cast<unsigned char>((argb & 0xFF000000) >> 12);
}

///////////////////////////////////////////////////////////////////////////////
unsigned char GetRed(unsigned argb)
{
	return static_cast<unsigned char>((argb & 0x00FF0000) >> 8);
}

///////////////////////////////////////////////////////////////////////////////
unsigned char GetGreen(unsigned argb)
{
	return static_cast<unsigned char>((argb & 0x0000FF00) >> 4);
}

///////////////////////////////////////////////////////////////////////////////
unsigned char GetBlue(unsigned argb)
{
	return static_cast<unsigned char>(argb & 0x000000FF);
}

///////////////////////////////////////////////////////////////////////////////
void SetAlpha(unsigned char alpha, unsigned& argb)
{
	argb |= ((unsigned)alpha << 12);
}

///////////////////////////////////////////////////////////////////////////////
void SetRed(unsigned char red, unsigned& argb)
{
	argb |= ((unsigned)red << 8);
}

///////////////////////////////////////////////////////////////////////////////
void SetGreen(unsigned char green, unsigned& argb)
{
	argb |= ((unsigned)green << 4);
}

///////////////////////////////////////////////////////////////////////////////
void SetBlue(unsigned char blue, unsigned& argb)
{
	argb |= (unsigned)blue;
}

///////////////////////////////////////////////////////////////////////////////