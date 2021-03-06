#pragma once

#include <iosfwd>
#include "OpenGL.h"

namespace Project {
namespace OpenGL {

#define PROJECT_OPENGL__COLOR_BIT_DEPTH 255

/** A RGBA color
*/
class Color
{
private:
	float r, g, b, a;
	static float colorIntToFloat(int i);
	static int colorFloatToInt(float f);
public:

	enum ColorPreset {RED,
		GREEN,
		BLUE,
		WHITE,
		BLACK,
		LIGHTGREY,
		GREY,
		DARKGREY,
		YELLOW,
		ORANGE,
		PURPLE,
		INDIGO,
		VIOLET,
		PINK,
		BROWN,
		TEAL};

	Color(void)
		: r(0.0f) , g(0.0f) , b(0.0f) , a(0.0f) {}
	Color(float red, float green, float blue, float alpha = 1.0f);
	Color(ColorPreset preset);
	~Color(void) {}

	static Color colori(int red, int green, int blue, int alpha = PROJECT_OPENGL__COLOR_BIT_DEPTH);

	float redf() const { return r; }
	float greenf() const { return g; }
	float bluef() const { return b; }
	float alphaf() const { return a; }
	int redi() const;
	int greeni() const;
	int bluei() const;
	int alphai() const;

	void setRedf(float red);
	void setGreenf(float green);
	void setBluef(float blue);
	void setAlphaf(float alpha);
	void setRedi(int red);
	void setGreeni(int green);
	void setBluei(int blue);
	void setAlphai(int alpha);
    
    int getRedi() const;
    int getGreeni() const;
    int getBluei() const;
    int getAlphai() const;
    
	void invert();

	bool isWhite() const { return (r == 1.0 && g == 1.0 && b == 1.0); }
	bool isBlack() const { return (r == 0.0 && g == 0.0 && b == 0.0); }

	/** Set this color as the drawing color in openGL
	*/
	void glApply() const;

	/** Set a color as the drawing color in openGL
	*/
	static void glColor(Color color);
	static void glColor(Color color, float force_alpha);

	Color operator + (const Color &other) const;
	Color operator - (const Color &other) const;
	Color operator * (const Color &other) const;
	Color operator / (const Color &other) const;
	Color operator * (float factor) const;
	Color operator / (float factor) const;
	bool operator == (const Color &other);
	void operator += (const Color &other);
	void operator -= (const Color &other);
	void operator *= (const Color &other);
	void operator /= (const Color &other);
	void operator *= (float factor);
	void operator /= (float factor);

	float operator [] (int index) const;

};

std::ostream &operator << (std::ostream &stream, const Color &color);
std::istream &operator >> (std::istream &stream, Color &color);

}  // namespace OpenGL
}  // namespace Project