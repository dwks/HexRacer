#include <cmath>

#include "TextWidget.h"
#include "NormalTextLayout.h"
#include "WidgetRenderer.h"

#include "render/FontManager.h"
#include "sdl/Projector.h"

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
    
    dirty = true;
    lastRenderedRatio = 0.0;
    preRender();
}

TextWidget::TextWidget(const std::string &name, const std::string &data,
    unsigned align, const WidgetRect &bounds, OpenGL::Color color)
    : AbstractWidget(name) {
    
    this->color = color;
    this->data = data;
    this->align = align;
    
    preRender();
    dirty = false;
    
    updateLayout(bounds);
}

TextWidget::~TextWidget() {
	if (stringTexture.hasTexture()) {
		glFinish();
		stringTexture.clearTexture();
	}
}

void TextWidget::preRender() {
    if (data.empty()) {

        if(getLayout() == NULL) {
            setLayout(new NormalTextLayout(align, 1.0));
        }
        
        return;
    }

	stringTexture.setString(
		data,
		Render::FontManager::getInstance()->getCurrentFont(),
		color,
		true);

	int w = stringTexture.getWidth();
	int h = stringTexture.getHeight();
    
	widthFactor = double(w) / (double) stringTexture.getOriginalWidth();
    heightFactor = double(h) / (double) stringTexture.getOriginalHeight();
    
	double aspectRatio = (double) stringTexture.getOriginalHeight()
        / (double) stringTexture.getOriginalWidth();
    aspectRatio *= SDL::Projector::getInstance()->getAspectRatio();
    
    if (getLayout() == NULL) {
        setLayout(new NormalTextLayout(align, aspectRatio));
    }
    else {
        NormalTextLayout *normal
            = dynamic_cast<NormalTextLayout *>(getLayout().get());
        
        if(normal) normal->setAspectRatio(aspectRatio);
        getLayout()->update();
    }
    
    dirty = false;
}

void TextWidget::render() {
    double ratioNow = SDL::Projector::getInstance()->getAspectRatio();
    if(lastRenderedRatio != ratioNow) {
        lastRenderedRatio = ratioNow;
        dirty = true;
    }
    
    if(dirty) {
        preRender();
    }
    
	if(data.empty() || !stringTexture.hasTexture()) {
        return;  // empty text, or error in pre-rendering
    }
    
    WidgetPoint corner = getBoundingRect().getCorner();
    WidgetPoint dimensions = getBoundingRect().getDimensions();
    
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, stringTexture.getTextureID());
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // comment this out to disable font transparency
    glEnable(GL_BLEND);
    
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
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
    dirty = true;
}

}  // namespace Widget
}  // namespace Project
