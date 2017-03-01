#include <stdafx.h>
#include "Color.h"

///////////////////////////////////////////////////////////////////////////////
const Color Color::White(1.0f, 1.0f, 1.0f);
const Color Color::Gray(0.5f, 0.5f, 0.5f);
const Color Color::Black(0.0f, 0.0f, 0.0f);

///////////////////////////////////////////////////////////////////////////////
Color::Color(float red, float green, float blue, float alpha)
	: Red(red)
	, Green(green)
	, Blue(blue)
	, Alpha(alpha)
{
}

///////////////////////////////////////////////////////////////////////////////
void Color::SetRedByte(unsigned char red)
{
	Red = float(red) / 255.0f;
}

///////////////////////////////////////////////////////////////////////////////
unsigned char Color::GetRedByte() const
{
	return static_cast<unsigned char>(Red * 255.0f);
}

///////////////////////////////////////////////////////////////////////////////
void Color::SetGreenByte(unsigned char green)
{
	Green = float(green) / 255.0f;
}

///////////////////////////////////////////////////////////////////////////////
unsigned char Color::GetGreenByte() const
{
	return static_cast<unsigned char>(Green * 255.0f);
}

///////////////////////////////////////////////////////////////////////////////
void Color::SetBlueByte(unsigned char blue)
{
	Blue = float(blue) / 255.0f;
}

///////////////////////////////////////////////////////////////////////////////
unsigned char Color::GetBlueByte() const
{
	return static_cast<unsigned char>(Blue * 255.0f);
}

///////////////////////////////////////////////////////////////////////////////
void Color::SetAlphaByte(unsigned char alpha)
{
	Alpha = float(alpha) / 255.0f;
}

///////////////////////////////////////////////////////////////////////////////
unsigned char Color::GetAlphaByte() const
{
	return static_cast<unsigned char>(Alpha * 255.0f);
}

///////////////////////////////////////////////////////////////////////////////
float Color::Brightness() const
{
	return (Red + Green + Blue) / 3.0f;
}

///////////////////////////////////////////////////////////////////////////////
Color& Color::Clip()
{
	float light = Red + Green + Blue;
	float excessLight = light - 3.0f;

	if (excessLight > 0.0)
	{
		Red += excessLight * Red / light;
		Green += excessLight * Green / light;
		Blue += excessLight * Blue / light;
	}

	Red = Clamp(Red, 0.0f, 1.0f);
	Green = Clamp(Green, 0.0f, 1.0f);
	Blue = Clamp(Blue, 0.0f, 1.0f);

	return *this;
}

///////////////////////////////////////////////////////////////////////////////
Color& Color::operator+= (float scalar)
{
	*this = *this + scalar;
	return *this;
}

///////////////////////////////////////////////////////////////////////////////
Color& Color::operator+= (const Color& c)
{
	*this = *this + c;
	return *this;
}

///////////////////////////////////////////////////////////////////////////////
Color& Color::operator*= (float scalar)
{
	*this = *this * scalar;
	return *this;
}

///////////////////////////////////////////////////////////////////////////////
Color& Color::operator*= (const Color& c)
{
	*this = *this * c;
	return *this;
}

///////////////////////////////////////////////////////////////////////////////
Color operator+ (const Color& c, float scalar)
{
	return Color(c.Red + scalar, c.Green + scalar, c.Blue + scalar, c.Alpha);
}

///////////////////////////////////////////////////////////////////////////////
Color operator+ (float scalar, const Color& c)
{
	return Color(c.Red + scalar, c.Green + scalar, c.Blue + scalar, c.Alpha);
}

///////////////////////////////////////////////////////////////////////////////
Color operator+ (const Color& a, const Color& b)
{
	return Color(a.Red + b.Red, a.Green + b.Green, a.Blue + b.Blue, a.Alpha + b.Alpha);
}

///////////////////////////////////////////////////////////////////////////////
Color operator* (const Color& c, float scalar)
{
	return Color(c.Red * scalar, c.Green * scalar, c.Blue * scalar, c.Alpha);
}

///////////////////////////////////////////////////////////////////////////////
Color operator* (float scalar, const Color& c)
{
	return Color(c.Red * scalar, c.Green * scalar, c.Blue * scalar, c.Alpha);
}

///////////////////////////////////////////////////////////////////////////////
Color operator* (const Color& a, const Color& b)
{
	return Color(a.Red * b.Red, a.Green * b.Green, a.Blue * b.Blue, a.Alpha * b.Alpha);
}

///////////////////////////////////////////////////////////////////////////////
Color Avg(const std::vector<Color>& colors)
{
	Color result = Color::Black;
	for (auto& c : colors)
	{
		result += c;
	}

	if (!colors.empty())
	{
		float sz = (float)colors.size();
		result.Red /= sz;
		result.Green /= sz;
		result.Blue /= sz;
		result.Alpha /= sz;
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////