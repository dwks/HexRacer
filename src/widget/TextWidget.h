#ifndef PROJECT_WIDGET__TEXT_WIDGET_H
#define PROJECT_WIDGET__TEXT_WIDGET_H

#include "AbstractWidget.h"
#include "NormalTextLayout.h"

#include "opengl/Color.h"
#include "render/StringTexture.h"

namespace Project {
namespace Widget {

class TextWidget : public AbstractWidget {
private:
    OpenGL::Color color;
    std::string data;
	Render::StringTexture stringTexture;
    double widthFactor, heightFactor;
    unsigned align;
    bool dirty;
public:
    TextWidget(const std::string &name)
        : AbstractWidget(name), dirty(false) {}
    TextWidget(const std::string &name, OpenGL::Color color,
        const std::string &data, unsigned align);
    TextWidget(const std::string &name, const std::string &data,
        unsigned align, const WidgetRect &bounds,
        OpenGL::Color color = OpenGL::Color::WHITE);
    ~TextWidget();
    
    void render();
    
    void setColor(OpenGL::Color color);
    
    const std::string &getData();
    void setText(const std::string &data);
    void addText(const std::string &add);
    
    virtual void accept(WidgetVisitor &visitor) { visitor.visit(this); }
private:
    
    void preRender();
    void textChanged();
};

}  // namespace Widget
}  // namespace Project

#endif
