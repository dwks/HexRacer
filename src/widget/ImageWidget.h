#ifndef PROJECT_WIDGET__IMAGE_WIDGET_H
#define PROJECT_WIDGET__IMAGE_WIDGET_H

#include "AbstractWidget.h"

#include <string>

namespace Project {
namespace Widget {

class ImageWidget : public AbstractWidget {
private:
    std::string filename;
    unsigned texture;
public:
    /**
        @param filename Can be empty if there is no image to display yet.
    */
    ImageWidget(const std::string &name, const std::string &filename,
        const WidgetRect &rect);
    
    void setFilename(const std::string &newFilename);
    
    void render();
    
    virtual void accept(WidgetVisitor &visitor) { visitor.visit(this); }
};

}  // namespace Widget
}  // namespace Project

#endif
