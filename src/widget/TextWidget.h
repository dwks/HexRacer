#ifndef PROJECT_WIDGET__TEXT_WIDGET_H
#define PROJECT_WIDGET__TEXT_WIDGET_H

#include "AbstractWidget.h"
#include "opengl/Color.h"

struct SDL_Surface;

namespace Project {
namespace Widget {

class TextWidget : public AbstractWidget {
private:
    OpenGL::Color color;
    std::string data;
    unsigned int texture;
    double widthFactor, heightFactor;
public:
    TextWidget(const std::string &name) : AbstractWidget(name), texture(-1) {}
    TextWidget(const std::string &name, OpenGL::Color color,
        const std::string &data);
    ~TextWidget();
    
    void render();
    
    virtual void accept(WidgetVisitor &visitor) { visitor.visit(this); }
private:
    int nextPowerOf2(int x);
    void preRender();
    void glVertex(const WidgetPoint &point);
};

}  // namespace Widget
}  // namespace Project

#endif
