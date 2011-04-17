#ifndef PROJECT_WIDGET__WIDGET_VISITOR_H
#define PROJECT_WIDGET__WIDGET_VISITOR_H

namespace Project {
namespace Widget {

class BoxWidget;
class TextWidget;
class ButtonWidget;
class CompositeWidget;
class EditWidget;
class ScrollbarWidget;
class ListWidget;
class ImageWidget;
class ProgressBarWidget;
class CheckWidget;
class ColourButtonWidget;

class WidgetVisitor {
public:
    virtual ~WidgetVisitor() {}
    
    virtual void visit(BoxWidget *widget) = 0;
    virtual void visit(TextWidget *widget) = 0;
    virtual void visit(ButtonWidget *widget) = 0;
    virtual void visit(CompositeWidget *widget) = 0;
    virtual void visit(EditWidget *widget) = 0;
    virtual void visit(ScrollbarWidget *widget) = 0;
    virtual void visit(ListWidget *widget) = 0;
    virtual void visit(ImageWidget *widget) = 0;
	virtual void visit(ProgressBarWidget *widget) = 0;
    virtual void visit(CheckWidget *widget) = 0;
    virtual void visit(ColourButtonWidget *widget) = 0;
};

}  // namespace Widget
}  // namespace Project

#endif
