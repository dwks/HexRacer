#ifndef PROJECT_OBJECT__PLAYER_H
#define PROJECT_OBJECT__PLAYER_H

#include "AbstractObject.h"
#include "math/Point.h"
#include "physics/PhysicalPlayer.h"
#include "render/RenderablePlayer.h"
#include "map/PathTracker.h"
#include "event/TogglePainting.h"
#include "world/PlayerIntention.h"
#include <string>

namespace Project {
namespace Object {

class Player : public AbstractObject {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void save(Archive &ar, const unsigned version) const {
        ar & boost::serialization::base_object<AbstractObject>(*this);
        ar & physical;
    }
    
    template <typename Archive>
    void load(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<AbstractObject>(*this);
        ar & physical;
        initialize();
    }
    
    BOOST_SERIALIZATION_SPLIT_MEMBER()
private:
    Physics::PhysicalPlayer *physical;
    Render::RenderablePlayer *renderable;
    Math::Point suspension[4];
	Map::PathTracker* tracker;
	Event::TogglePainting::PaintType paintType;
    ::Project::World::PlayerIntention intention;
public:
    Player();
	Player(int id, const Math::Point &origin,
        const Math::Point &direction = Math::Point(0.0, 0.0, 1.0));
    virtual ~Player();
    
    void setOnGround(bool onGround)
        { physical->setOnGround(onGround); }
    bool getOnGround() const { return physical->getOnGround(); }
    
    void setSpeedBoost(double speedBoost)
        { physical->setSpeedBoost(speedBoost); }
    double getSpeedBoost(){
        return physical->getSpeedBoost();    
    }
    
    void setIntention(const ::Project::World::PlayerIntention &intention)
        { this->intention = intention; }
    const ::Project::World::PlayerIntention &getIntention() const
        { return intention; }
    
    void setPhysicalObject(Physics::PhysicalPlayer *physical)
        { this->physical = physical; }
    virtual Physics::PhysicalPlayer *getPhysicalObject();
    
    void setRenderableObject(Render::RenderablePlayer *renderable)
        { this->renderable = renderable; }
    virtual Render::RenderableObject *getRenderableObject();

	void setPathTracker(Map::PathTracker* _tracker)
		{ tracker = _tracker; }
	Map::PathTracker* getPathTracker()
		{ return tracker; }

	const Map::PathTracker* getPathTracker() const
		{ return tracker; }
    
    void applyAcceleration(double acceleration)
        { physical->applyAcceleration(acceleration); }
    void applyTurning(double amount)
        { physical->applyTurning(amount); }
    void doJump() { physical->doJump(); }
    
    void setSuspension(int wheel, Math::Point suspension)
        { this->suspension[wheel] = suspension; }
    Math::Point getSuspension(int wheel) { return suspension[wheel]; }
    
    Math::Point getPosition() const;
    Math::Matrix getTransformation() const;
    void applyForce(const Math::Point &movement, const Math::Point &at);

	Event::TogglePainting::PaintType getPaintType() const { return paintType; }
	void setPaintType(Event::TogglePainting::PaintType paint_type) { paintType = paint_type; }

	int getTeamID() const { return (getID()/2 % 8); }
	std::string getPlayerName() const;

	bool operator < (const Player& other) const;
    
    void initialize();
    
    virtual void preRender();
};

}  // namespace Object
}  // namespace Project

#endif
