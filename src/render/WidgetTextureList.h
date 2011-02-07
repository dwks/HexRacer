#ifndef PROJECT_RENDER__WIDGET_TEXTURE_LIST_H
#define PROJECT_RENDER__WIDGET_TEXTURE_LIST_H

#include <map>
#include <string>

#include "opengl/OpenGL.h"

namespace Project {
namespace Render {
    
class WidgetTextureList {
private:
    static WidgetTextureList *instance;
public:
    static WidgetTextureList *getInstance() { return instance; }
private:
    typedef std::map<std::string, GLuint> texture_list_t;
    texture_list_t textureList;
public:
    WidgetTextureList();
    
    void addTexture(const std::string &name, GLuint texture);
    GLuint getTexture(const std::string &name);
private:
    void addTexture(const std::string &name, const std::string &filename);
};

}  // namespace Render
}  // namespace Project

#endif
