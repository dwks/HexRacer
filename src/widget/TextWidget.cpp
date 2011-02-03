#include <cmath>

#include "TextWidget.h"

#include "render/FontManager.h"

#include "SDL.h"
#include "SDL_ttf.h"
#include "opengl/OpenGL.h"

namespace Project {
namespace Widget {

TextWidget::TextWidget(const std::string &name, OpenGL::Color color,
    const std::string &data)
    : AbstractWidget(name) {
    
    this->color = color;
    this->data = data;
    texture = -1;
    
    preRender();
}

TextWidget::~TextWidget() {
    if(texture != unsigned(-1)) {
        // make sure we're finished rendering before deleting the texture
        glFinish();
        
        glDeleteTextures(1, &texture);
    }
}

int TextWidget::nextPowerOf2(int x) {
    return 1 << int(std::ceil(std::log(x) / std::log(2.0)));
}

void TextWidget::preRender() {
    SDL_Color c;
    c.r = color.getRedi();
    c.g = color.getGreeni();
    c.b = color.getBluei();
    
    TTF_Font *font = Render::FontManager::getInstance()->getCurrentFont();
    
    // note: we rely on this function creating 32-bit images,
    // as its documentation describes.
    SDL_Surface *first = TTF_RenderText_Blended(font, data.c_str(), c);
    
    int w = nextPowerOf2(first->w);
    int h = nextPowerOf2(first->h);
    
    SDL_Surface *second = SDL_CreateRGBSurface(
        SDL_SWSURFACE, w, h,
        32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    
    // copy data from first to second, translating to new format
    SDL_BlitSurface(first, NULL, second, NULL);
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // we assume this copies the pixels from second, and later free second
    glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA, 
        GL_UNSIGNED_BYTE, second->pixels);
    
    SDL_FreeSurface(first);
    SDL_FreeSurface(second);
}

void TextWidget::render() {
    WidgetPoint corner = getBoundingRect().getCorner();
    WidgetPoint dimensions = getBoundingRect().getDimensions();
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glBegin(GL_QUADS);
    
    // OpenGL origin in lower left corner
    glTexCoord2i(0, 1); this->glVertex(corner);
    glTexCoord2i(1, 1); this->glVertex(corner.plusXOf(dimensions));
    glTexCoord2i(1, 0); this->glVertex(corner + dimensions);
    glTexCoord2i(0, 0); this->glVertex(corner.plusYOf(dimensions));
    
    glEnd();
}

void TextWidget::glVertex(const WidgetPoint &point) {
    glVertex2d(point.getX(), point.getY());
}

}  // namespace Widget
}  // namespace Project
