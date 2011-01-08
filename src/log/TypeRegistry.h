#ifndef PROJECT_LOG__TYPE_REGISTRY_H
#define PROJECT_LOG__TYPE_REGISTRY_H

#include <string>
#include <map>
#include "Subsystem.h"
#include "misc/Singleton.h"

namespace Project {
namespace Log {

class TypeRegistry /*: public Misc::Singleton<TypeRegistry>*/ {
private:
	static TypeRegistry instance;
public:
	static TypeRegistry &getInstance() { return instance; }
private:
    typedef std::map<std::string, int> type_list_t;
    type_list_t type_list[Subsystem::SUBSYSTEMS];
protected:
    friend class Misc::Singleton<TypeRegistry>;
    TypeRegistry();
    void registerType(Subsystem subsystem,
        const std::string &type, int level);
public:
    bool isValidType(Subsystem subsystem, const std::string &type);
    int getLevelOfType(Subsystem subsystem, const std::string &type);
};

}  // namespace Log
}  // namespace Project

#endif
