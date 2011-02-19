#ifndef PROJECT_WIDGET__TEXT_WIDGET_H
#define PROJECT_WIDGET__TEXT_WIDGET_H

#include "AbstractWidget.h"
#include "NormalTextLayout.h"

#include "opengl/Color.h"

namespace Project {
namespace Widget {

class TextWidget : public AbstractWidget {
private:
    OpenGL::Color color;
    std::string data;
    unsigned int texture;
    double widthFactor, heightFactor;
    unsigned align;
    bool dirty;
public:
    TextWidget(const std::string &name)
        : AbstractWidget(name), texture(-1), dirty(false) {}
    TextWidget(const std::string &name, OpenGL::Color color,
        const std::string &data, unsigned align);
    ~TextWidget();
    
    void render();
    
    const std::string &getData();
    void setText(const std::string &data);
    void addText(const std::string &add);
    
    virtual void accept(WidgetVisitor &visitor) { visitor.visit(this); }
private:
    int nextPowerOf2(int x);
    void preRender();
    void textChanged();
};

}  // namespace Widget
}  // namespace Project

#endif
