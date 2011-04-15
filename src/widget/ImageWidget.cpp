#include "ImageWidget.h"
#include "NormalTextLayout.h"
#include "SmoothTransitionLayout.h"

#include "WidgetRenderer.h"

#include "opengl/TextureLoading.h"
#include "sdl/Projector.h"

#include "log/Logger.h"

namespace Project {
namespace Widget {

ImageWidget::ImageWidget(const std::string &name, const std::string &filename,
    const WidgetRect &rect) : AbstractWidget(name) {
    
    setLayout(new NormalTextLayout(0, 1.0));
    updateLayout(rect);
    
    if(!filename.empty()) setFilename(filename);
    
    properRatio = 0.0;
    lastRenderedRatio = 1.0;
}

void ImageWidget::setFilename(const std::string &newFilename) {
    this->filename = newFilename;

	int w;
	int h;
    
    texture = OpenGL::TextureLoading::loadTexture2D(filename, GL_REPEAT, GL_REPEAT,
		GL_LINEAR, GL_LINEAR, false, &w, &h);

    LOG(WIDGET, "Loading \"" << filename << "\", ID is " << texture);
    
    properRatio = static_cast<double>(h) / w;
    LOG(WIDGET, "properRatio = " << h << "/" << w << " = " << properRatio);
    lastRenderedRatio = -1.0;
    dynamic_cast<NormalTextLayout *>(getLayout().get())->setAspectRatio(properRatio);
    updateLayout();
}

void ImageWidget::render() {
    if(filename.empty()) {
        return;
    }
    
    double ratioNow = SDL::Projector::getInstance()->getAspectRatio();
    if(lastRenderedRatio != ratioNow) {
        NormalTextLayout *normal;
        SmoothTransitionLayout *smooth
            = dynamic_cast<SmoothTransitionLayout *>(getLayout().get());
        if(smooth) {
            normal = dynamic_cast<NormalTextLayout *>(smooth->getNormalLayout().get());
        }
        else {
            normal = dynamic_cast<NormalTextLayout *>(getLayout().get());
        }
        
        if(normal) {
            LOG(GUI, "Setting aspect ratio to " << properRatio << "*" << ratioNow);
            normal->setAspectRatio(properRatio * ratioNow);
            updateLayout();
        }
        
        lastRenderedRatio = ratioNow;
    }
    
    WidgetPoint corner = getBoundingRect().getCorner();
    WidgetPoint dimensions = getBoundingRect().getDimensions();
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // comment this out to disable image transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBegin(GL_QUADS);
    
    double width = dimensions.getX();
    double height = dimensions.getY();
    
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

}  // namespace Widget
}  // namespace Project
