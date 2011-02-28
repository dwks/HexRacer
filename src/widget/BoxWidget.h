#ifndef PROJECT_WIDGET__BOX_WIDGET_H
#define PROJECT_WIDGET__BOX_WIDGET_H

#include "AbstractWidget.h"

namespace Project {
namespace Widget {

class BoxWidget : public AbstractWidget {
private:
    std::string artwork;
public:
    BoxWidget(const std::string &name,
        const std::string &initialArtwork = "corners/out/normal")
        : AbstractWidget(name), artwork(initialArtwork) {}
    
    void setArtwork(const std::string &artwork);
    
    void render();
    
    virtual void accept(WidgetVisitor &visitor) { visitor.visit(this); }
private:
    void drawQuad(double xmin, double xmax, double ymin, double ymax,
        WidgetPoint corner, WidgetPoint dimensions);
};

}  // namespace Widget
}  // namespace Project

#endif
