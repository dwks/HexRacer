#include "Color.h"
#include "math/Values.h"
#include "OpenGL.h"
#include <ostream>
#include <istream>
using namespace Project;
using namespace Math;

namespace Project {
namespace OpenGL {

Color::Color(float red, float green, float blue, float alpha) {
	setRedf(red);
	setGreenf(green);
	setBluef(blue);
	setAlphaf(alpha);
}

/*
Color::Color(Point p, float alpha) {
	setRedf((float) p.getX());
	setGreenf((float) p.getY());
	setBluef((float) p.getZ());
	setAlphaf(alpha);
}
*/

Color Color::colori(int red, int green, int blue, int alpha) {
	Color c;
	c.setRedi(red);
	c.setGreeni(green);
	c.setBluei(blue);
	c.setAlphai(alpha);
	return c;
}



Color::Color(ColorPreset preset) {
	switch (preset) {
		case RED:
			r = 1.0f;
			g = 0.0f;
			b = 0.0f;
			;break;
		case GREEN:
			r = 0.0f;
			g = 1.0f;
			b = 0.0f;
			;break;
		case BLUE:
			r = 0.0f;
			g = 0.0f;
			b = 1.0f;
			;break;
		case WHITE:
			r = 1.0f;
			g = 1.0f;
			b = 1.0f;
			;break;
		case BLACK:
			r = 0.0f;
			g = 0.0f;
			b = 0.0f;
			;break;
		case LIGHTGREY:
			r = 0.75f;
			g = 0.75f;
			b = 0.75f;
			;break;
		case GREY:
			r = 0.5f;
			g = 0.5f;
			b = 0.5f;
			;break;
		case DARKGREY:
			r = 0.25f;
			g = 0.25f;
			b = 0.25f;
			;break;
		case YELLOW:
			r = 1.0f;
			g = 1.0f;
			b = 0.0f;
			;break;
		case ORANGE:
			r = 1.0f;
			g = 0.5f;
			b = 0.0f;
			;break;
		case PURPLE:
			r = 0.5f;
			g = 0.0f;
			b = 0.5f;
			;break;
		case INDIGO:
			r = 0.294f;
			g = 0.0f;
			b = 0.510f;
			;break;
		case VIOLET:
			r = 0.5f;
			g = 0.0f;
			b = 1.0f;
			;break;
		case PINK:
			r = 1.0f;
			g = 0.753f;
			b = 0.796f;
			;break;
		case BROWN:
			r = 0.588f;
			g = 0.294f;
			b = 0.0f;
			;break;
		case TEAL:
			r = 0.212f;
			g = 0.459f;
			b = 0.533f;
			;break;
	}
	
	a = 1.0f;
}

float Color::colorIntToFloat(int i) {
	return (float) i/ (float) PROJECT_OPENGL__COLOR_BIT_DEPTH;
}

int Color::colorFloatToInt(float f) {
	return (int) (f*(float) PROJECT_OPENGL__COLOR_BIT_DEPTH);
}

int Color::redi() const {
	return colorFloatToInt(redf());
}

int Color::greeni() const {
	return colorFloatToInt(greenf());
}

int Color::bluei() const {
	return colorFloatToInt(bluef());
}

int Color::alphai() const {
	return colorFloatToInt(alphaf());
}

void Color::setRedf(float red) {
	r = bound(red, 0.0f, 1.0f);
}

void Color::setGreenf(float green) {
	g = bound(green, 0.0f, 1.0f);
}

void Color::setBluef(float blue) {
	b = bound(blue, 0.0f, 1.0f);
}

void Color::setAlphaf(float alpha) {
	a = bound(alpha, 0.0f, 1.0f);
}

void Color::setRedi(int red) {
	setRedf(colorIntToFloat(red));
}

void Color::setGreeni(int green) {
	setGreenf(colorIntToFloat(green));
}

void Color::setBluei(int blue) {
	setBluef(colorIntToFloat(blue));
}

void Color::setAlphai(int alpha) {
	setAlphaf(colorIntToFloat(alpha));
}

int Color::getRedi() const {
    return r * PROJECT_OPENGL__COLOR_BIT_DEPTH;
}

int Color::getGreeni() const {
    return g * PROJECT_OPENGL__COLOR_BIT_DEPTH;
}

int Color::getBluei() const {
    return b * PROJECT_OPENGL__COLOR_BIT_DEPTH;
}

int Color::getAlphai() const {
    return a * PROJECT_OPENGL__COLOR_BIT_DEPTH;
}

void Color::invert() {
	setRedf(1.0f-redf());
	setGreenf(1.0f-greenf());
	setBluef(1.0f-bluef());
	setAlphaf(1.0f-alphaf());
}

/*
GLfloat[4] Color::toGLfloat() {
	GLfloat glf[4] = {red(), green(), blue(), alpha()};
	return glf;
}
*/

Color Color::operator + (const Color &other) const {
	return Color(
		redf()+other.redf(),
		greenf()+other.greenf(),
		bluef()+other.bluef(),
		alphaf()+other.alphaf()
	);
}

Color Color::operator - (const Color &other) const {
	return Color(
		redf()-other.redf(),
		greenf()-other.greenf(),
		bluef()-other.bluef(),
		alphaf()-other.alphaf()
	);
}

Color Color::operator * (const Color &other) const {
	return Color(
		redf()*other.redf(),
		greenf()*other.greenf(),
		bluef()*other.bluef(),
		alphaf()*other.alphaf()
	);
}

Color Color::operator / (const Color &other) const {
	return Color(
		redf()/other.redf(),
		greenf()/other.greenf(),
		bluef()/other.bluef(),
		alphaf()/other.alphaf()
	);
}

Color Color::operator * (float factor) const {
	return Color(
		redf()*factor,
		greenf()*factor,
		bluef()*factor,
		alphaf()*factor
	);
}

Color Color::operator / (float factor) const {
	return Color(
		redf()/factor,
		greenf()/factor,
		bluef()/factor,
		alphaf()/factor
	);
}

bool Color::operator == (const Color &other) {
	return (r == other.redf() && g == other.greenf() && b == other.bluef());
}
void Color::operator += (const Color &other) {
	setRedf(redf()+(other.redf()));
	setGreenf(greenf()+other.greenf());
	setBluef(bluef()+other.bluef());
	setAlphaf(alphaf()+other.alphaf());
}

void Color::operator -= (const Color &other) {
	setRedf(redf()-other.redf());
	setGreenf(greenf()-other.greenf());
	setBluef(bluef()-other.bluef());
	setAlphaf(alphaf()-other.alphaf());
}

void Color::operator *= (const Color &other) {
	setRedf(redf()*other.redf());
	setGreenf(greenf()*other.greenf());
	setBluef(bluef()*other.bluef());
	setAlphaf(alphaf()*other.alphaf());
}

void Color::operator /= (const Color &other) {
	setRedf(redf()/other.redf());
	setGreenf(greenf()/other.greenf());
	setBluef(bluef()/other.bluef());
	setAlphaf(alphaf()/other.alphaf());
}

void Color::operator *= (float factor) {
	setRedf(redf()*factor);
	setGreenf(greenf()*factor);
	setBluef(bluef()*factor);
	setAlphaf(alphaf()*factor);
}

void Color::operator /= (float factor) {
	setRedf(redf()/factor);
	setGreenf(greenf()/factor);
	setBluef(bluef()/factor);
	setAlphaf(alphaf()/factor);
}

float Color::operator [] (int index) const {
	switch (index) {
		case 0: return redf();
		case 1: return greenf();
		case 2: return bluef();
		case 3: return alphaf();
		default: return 0;
	}
}
void Color::glApply() const {
	glColor(*this);
}

void Color::glColor(Color color) {
	glColor4f(color.redf(), color.greenf(), color.bluef(), color.alphaf());
}

void Color::glColor(Color color, float force_alpha) {
	glColor4f(color.redf(), color.greenf(), color.bluef(), force_alpha);
}

std::ostream &operator << (std::ostream &stream, const Color &color) {
    stream << '('
		<< color.redi() << ','
        << color.greeni() << ','
        << color.bluei() << ','
		<< color.alphai() << ')';
    
    return stream;
}

std::istream &operator >> (std::istream &stream, Color &color) {
    int r, g, b, a;
    char c;
    stream >> c >> r >> c >> g >> c >> b >> c >> a >> c;
    
	color.setRedi(r);
	color.setGreeni(g);
	color.setBluei(b);
	color.setAlphai(a);

	return stream;
}

}  // namespace OpenGL
}  // namespace Project