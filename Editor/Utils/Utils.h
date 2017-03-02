#pragma once

///////////////////////////////////////////////////////////////////////////////
wxBitmapType PathToBmpType(const wxString& path);

std::vector<concurrency::graphics::float_4> ImgToFloat4(const wxImage& img);
void Float4ToImg(const std::vector<concurrency::graphics::float_4>& from, wxImage& to);

std::vector<unsigned> ImgToARGB(const wxImage& img);
void ARGBToImg(const std::vector<unsigned>& from, wxImage& to);

unsigned char GetAlpha(unsigned argb);
unsigned char GetRed(unsigned argb);
unsigned char GetGreen(unsigned argb);
unsigned char GetBlue(unsigned argb);

void SetAlpha(unsigned char alpha, unsigned& argb);
void SetRed(unsigned char red, unsigned& argb);
void SetGreen(unsigned char green, unsigned& argb);
void SetBlue(unsigned char blue, unsigned& argb);

///////////////////////////////////////////////////////////////////////////////

