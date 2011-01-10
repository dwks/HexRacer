#pragma once

#include "../math/Point.h"

namespace Project {
namespace OpenGL {

#define PROJECT_OPENGL__COLOR_BIT_DEPTH 255

class Color
{
private:
	float r, g, b, a;
	float colorIntToFloat(int i);
	int colorFloatToInt(float f);
public:

	enum ColorPreset {RED, GREEN, BLUE, WHITE, BLACK, LIGHTGREY, GREY, DARKGREY, YELLOW, ORANGE, PURPLE, INDIGO, VIOLET, PINK, BROWN, TEAL};

	Color(void)
		: r(0.0f) , g(0.0f) , b(0.0f) , a(0.0f) {}
	Color(float red, float green, float blue, float alpha = 1.0f);
	Color(Math::Point p, float alpha = 1.0f);
	Color(ColorPreset preset);
	~Color(void) {}

	static Color colori(int red, int green, int blue, int alpha = PROJECT_OPENGL__COLOR_BIT_DEPTH);

	float redf() const { return r; }
	float greenf() const { return g; }
	float bluef() const { return b; }
	float alphaf() const { return a; }
	int redi();
	int greeni();
	int bluei();
	int alphai();

	void setRedf(float red);
	void setGreenf(float green);
	void setBluef(float blue);
	void setAlphaf(float alpha);
	void setRedi(int red);
	void setGreeni(int green);
	void setBluei(int blue);
	void setAlphai(int alpha);

	void invert();

	static void glColor(Color color);
	static void glColor(Color color, double force_alpha);

	Color operator + (const Color &other) const;
	Color operator - (const Color &other) const;
	Color operator * (const Color &other) const;
	Color operator / (const Color &other) const;
	Color operator * (double factor) const;
	Color operator / (double factor) const;
	void operator += (const Color &other);
	void operator -= (const Color &other);
	void operator *= (const Color &other);
	void operator /= (const Color &other);
	void operator *= (double factor);
	void operator /= (double factor);

};

}  // namespace OpenGL
}  // namespace Project