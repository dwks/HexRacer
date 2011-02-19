#ifndef PROJECT_WIDGET__EDIT_WIDGET_H
#define PROJECT_WIDGET__EDIT_WIDGET_H

#include "AbstractWidget.h"
#include "BoxWidget.h"
#include "TextWidget.h"

namespace Project {
namespace Widget {

class EditWidget : public AbstractWidget {
private:
    BoxWidget *box;
    TextWidget *text;
public:
    EditWidget(const std::string &name, const std::string &initialData,
        const WidgetRect &bounds);
    virtual ~EditWidget();
    
    BoxWidget *getBox() { return box; }
    
    /** Returns a pointer to the internal TextWidget object.
        
        Note: code outside the Widget namespace should have no need to call
        this function. Use getData() to get the contents of this EditWidget.
    */
    TextWidget *getText() { return text; }
    
    /** Sets the text stored in this EditWidget.
    */
    void setData(const std::string &data);
    
    /** Returns the data currently stored in this EditWidget.
    */
    const std::string &getData();
    
    void addCharacter(char add);
    
    virtual void accept(WidgetVisitor &visitor) { visitor.visit(this); }
};

}  // namespace Widget
}  // namespace Project

#endif
