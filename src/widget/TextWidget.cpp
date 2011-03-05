#include <cmath>

#include "TextWidget.h"
#include "NormalTextLayout.h"
#include "WidgetRenderer.h"

#include "render/FontManager.h"

#include "SDL.h"
#include "SDL_ttf.h"
#include "opengl/OpenGL.h"

#include "log/Logger.h"

namespace Project {
namespace Widget {

TextWidget::TextWidget(const std::string &name, OpenGL::Color color,
    const std::string &data, unsigned align)
    : AbstractWidget(name) {
    
    this->color = color;
    this->data = data;
    this->align = align;
    texture = -1;
    
    dirty = true;
    preRender();
}

TextWidget::TextWidget(const std::string &name, const std::string &data,
    unsigned align, const WidgetRect &bounds)
    : AbstractWidget(name) {
    
    this->color = OpenGL::Color::WHITE;
    this->data = data;
    this->align = align;
    texture = -1;
    
    preRender();
    dirty = false;
    
    updateLayout(bounds);
}

TextWidget::~TextWidget() {
    if(texture != unsigned(-1)) {
        // make sure we're finished rendering before deleting the texture
        glFinish();
        
        glDeleteTextures(1, &texture);
    }
}

int TextWidget::nextPowerOf2(int x) {
    return 1 << int(std::ceil((double) std::log((double) x) / std::log((double) 2.0)));
}

void TextWidget::preRender() {
    if(data.empty()) {
        texture = -1;
        
        if(getLayout() == NULL) {
            setLayout(new NormalTextLayout(align, 1.0));
        }
        
        return;
    }
    
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
    
    widthFactor = double(w) / first->w;
    heightFactor = double(h) / first->h;
    
    SDL_Surface *second = SDL_CreateRGBSurface(
        SDL_SWSURFACE, w, h,
        32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    
    // copy data from first to second, translating to new format
    SDL_BlitSurface(first, NULL, second, NULL);
    
    glEnable(GL_TEXTURE_2D);
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // we assume this copies the pixels from second, and later free second
    glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA, 
        GL_UNSIGNED_BYTE, second->pixels);
    
    //glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    
    double aspectRatio = double(first->h) / first->w;
    
    if(getLayout() == NULL) {
        setLayout(new NormalTextLayout(align, aspectRatio));
    }
    else {
        static_cast<NormalTextLayout *>(getLayout())
            ->setAspectRatio(aspectRatio);
        getLayout()->update();
    }
    
    SDL_FreeSurface(first);
    SDL_FreeSurface(second);
    
    dirty = false;
}

void TextWidget::render() {
    if(dirty) {
        preRender();
    }
    
    if(texture == unsigned(-1)) {
        return;  // empty text, or error in pre-rendering
    }
    
    WidgetPoint corner = getBoundingRect().getCorner();
    WidgetPoint dimensions = getBoundingRect().getDimensions();
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // comment this out to disable font transparency
    glEnable(GL_BLEND);
    
    glBlendFunc(GL_ONE, GL_ONE);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
    
    glBegin(GL_QUADS);
    
    double width = dimensions.getX() * widthFactor;
    double height = dimensions.getY() * heightFactor;
    
    // scale dimensions to take the larger power-of-2 texture into account
    // this way it looks the same no matter how big the texture actually is
    WidgetPoint topLeft = corner;
    WidgetPoint lowerLeft = corner;
    WidgetPoint topRight = corner;
    WidgetPoint lowerRight = corner;
    lowerLeft.addY(height);
    lowerRight.addY(height);
    topRight.addX(width);
    lowerRight.addX(width);
    
    glTexCoord2i(0, 0); WidgetRenderer::glVertex(topLeft);
    glTexCoord2i(1, 0); WidgetRenderer::glVertex(topRight);
    glTexCoord2i(1, 1); WidgetRenderer::glVertex(lowerRight);
    glTexCoord2i(0, 1); WidgetRenderer::glVertex(lowerLeft);
    
    glEnd();
    
    // back to the blending the rest of the code expects
    // note: must match SDLMain
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

void TextWidget::setColor(OpenGL::Color color) {
    this->color = color;
    textChanged();
}

const std::string &TextWidget::getData() {
    return data;
}

void TextWidget::addText(const std::string &add) {
    this->data += add;
    textChanged();
}

void TextWidget::setText(const std::string &data) {
    if(data != this->data) {
        this->data = data;
        textChanged();
    }
}

void TextWidget::textChanged() {
    //LOG(WIDGET, "Text widget data set to \"" << data << "\"");
    
    if(texture != unsigned(-1)) {
        // make sure we're finished rendering before deleting the texture
        glFinish();
        
        glDeleteTextures(1, &texture);
        texture = -1;
    }
    
    dirty = true;
}

}  // namespace Widget
}  // namespace Project
