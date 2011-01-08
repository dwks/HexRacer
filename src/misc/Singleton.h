#ifndef PROJECT_MISC__SINGLETON_H
#define PROJECT_MISC__SINGLETON_H

#include <cstddef>  // for NULL

namespace Project {
namespace Misc {

template <typename ClassType>
class Singleton {
private:
    static ClassType *instance;
protected:
    /** Protected default constructor.
    */
    Singleton() {}
    
    /** Protected copy constructor.
    */
    Singleton(const Singleton<ClassType> &) {}
    
    /** Protected assignment operator.
    */
    void operator = (const Singleton<ClassType> &) {}
public:
    static ClassType &getInstance();
};

template <typename ClassType>
ClassType &Singleton<ClassType>::getInstance() {
    if(instance == NULL) {
        instance = new ClassType();
    }
    
    return *instance;
}

}  // namespace Misc
}  // namespace Project

#endif
