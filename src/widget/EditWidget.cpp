#include "EditWidget.h"
#include "AbsoluteLayout.h"
#include "EditEventProxy.h"

namespace Project {
namespace Widget {

EditWidget::EditWidget(const std::string &name, const std::string &initialData,
    const WidgetRect &bounds) : AbstractWidget(name) {
    
    box = new BoxWidget("edit-box", "corners/in/normal");
    box->setLayout(new AbsoluteLayout(bounds));
    
    oldText = initialData;
    text = new TextWidget("edit-text", OpenGL::Color::WHITE, initialData,
        NormalTextLayout::ALIGN_LEFT | NormalTextLayout::ALIGN_VCENTRE);
    WidgetRect textBounds = bounds;
    textBounds.getCorner() += textBounds.getDimensions() * 0.15;
    textBounds.getDimensions() *= 0.7;
    text->updateLayout(textBounds);
    
    setLayout(new AbsoluteLayout(bounds));
    
    addEventProxy(new EditEventProxy(this));
}

EditWidget::~EditWidget() {
    delete box;
    delete text;
}

void EditWidget::setData(const std::string &data) {
    text->setText(data);
}

const std::string &EditWidget::getData() {
    return text->getData();
}

void EditWidget::saveOldText() {
    oldText = text->getData();
}

void EditWidget::restoreOldText() {
    text->setText(oldText);
}

bool EditWidget::oldTextChanged() {
    return oldText != text->getData();
}

void EditWidget::addCharacter(char add) {
    std::string addString;
    addString += add;
    text->addText(addString);
}

}  // namespace Widget
}  // namespace Project
