#ifndef PROJECT_MISC__DELETE_FUNCTOR_H
#define PROJECT_MISC__DELETE_FUNCTOR_H

namespace Project {
namespace Misc {

/** Functor to delete an object. This can be used to free all members of an
    STL container at once, with syntax like
        std::for_each(container.begin(), container.end(),
            DeleteFunctor<Type>());
    Note that std::for_each is in <algorithm>.
*/
template <typename Type>
class DeleteFunctor {
public:
    void operator () (Type *object) {
        delete object;
    }
};

}  // namespace Misc
}  // namespace Project

#endif
