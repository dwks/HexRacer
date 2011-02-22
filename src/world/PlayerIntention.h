#ifndef PROJECT_WORLD__PLAYER_INTENTION_H
#define PROJECT_WORLD__PLAYER_INTENTION_H

namespace Project {
namespace World {

/** Encompasses all the inputs of a player (human or computer) for one frame.
*/
class PlayerIntention {
private:
    double accel;
    double turn;
    bool jump;
    bool reset;
    bool paint;
    bool erase;
public:
    PlayerIntention() : accel(0.0), turn(0.0), jump(false), reset(false),
        paint(false), erase(false) {}
    
    double getAccel() const { return accel; }
    double getTurn() const { return turn; }
    bool getJump() const { return jump; }
    bool getReset() const { return reset; }
    bool getPaint() const { return paint; }
    bool getErase() const { return erase; }
    
    void setAccel(double accel) { this->accel = accel; }
    void setTurn(double turn) { this->turn = turn; }
    void setJump(bool jump) { this->jump = jump; }
    void setReset(bool reset) { this->reset = reset; }
    void setPaint(bool paint) { this->paint = paint; }
    void setErase(bool erase) { this->erase = erase; }
};

}  // namespace World
}  // namespace Project

#endif
