#ifndef PROJECT_PHYSICS__WARP_TRACKER_H
#define PROJECT_PHYSICS__WARP_TRACKER_H

#include "boost/serialization/access.hpp"

namespace Project {
namespace Physics {

class WarpTracker {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & counting;
        ar & countingSince;
    }
private:
    enum type_t {
        OFF_TRACK,
        SITTING_STILL,
        REQUEST_HELP,
        TYPES
    };
private:
    bool counting[TYPES];
    unsigned long countingSince[TYPES];
public:
    WarpTracker();
    
    void setOffTrack(bool current);
    void setSittingStill(bool current);
    void setRequestingHelp(bool current);
    
    bool canReceiveWarp();
    
    void clear();
private:
    void doCounting(type_t type, bool current);
};

}  // namespace Physics
}  // namespace Project

#endif
