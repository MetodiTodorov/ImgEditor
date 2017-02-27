#pragma once
///////////////////////////////////////////////////////////////////////////////
#include <vector>

///////////////////////////////////////////////////////////////////////////////
class Color
{
public:
	Color(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f);

	void			SetRedByte(unsigned char red);
	unsigned char	GetRedByte() const;
	void			SetGreenByte(unsigned char green);
	unsigned char	GetGreenByte() const;
	void			SetBlueByte(unsigned char blue);
	unsigned char	GetBlueByte() const;
	void			SetAlphaByte(unsigned char alpha);
	unsigned char	GetAlphaByte() const;

	float			Brightness() const;
	Color&			Clip();

	Color&			operator+= (float scalar);
	Color&			operator+= (const Color& c);
	Color&			operator*= (float scalar);
	Color&			operator*= (const Color& c);

	static const Color White;
	static const Color Gray;
	static const Color Black;

public:
	float Red;
	float Green;
	float Blue;
	float Alpha;
};

///////////////////////////////////////////////////////////////////////////////
Color operator+ (const Color& c, float scalar);
Color operator+ (float scalar, const Color& c);
Color operator+ (const Color& a, const Color& b);
Color operator* (const Color& c, float scalar);
Color operator* (float scalar, const Color& c);
Color operator* (const Color& a, const Color& b);

///////////////////////////////////////////////////////////////////////////////
Color Avg(const std::vector<Color>& colors);

///////////////////////////////////////////////////////////////////////////////