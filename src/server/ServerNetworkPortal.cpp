#include "ServerNetworkPortal.h"
#include "network/EventPacket.h"

#include "event/EventSystem.h"
#include "event/ChangeOfIntention.h"

#include "log/Logger.h"

namespace Project {
namespace Server {

ServerNetworkPortal::ServerNetworkPortal(ClientManager *clientManager)
    : clientManager(clientManager) {
    
    ADD_OBSERVER(new EventPropagator(this));
}

void ServerNetworkPortal::EventPropagator::observe(Event::EventBase *event) {
    switch(event->getType()) {
    case Event::EventType::CHANGE_OF_INTENTION: {
        Event::ChangeOfIntention *changeOfIntention
            = dynamic_cast<Event::ChangeOfIntention *>(event);
        changeOfIntention->setPropagate(false);
        
        Network::Packet *packet = new Network::EventPacket(event);
        
        if(event->getType() < 0) {
            portal->getClientManager()->sendPacket(packet);
        }
        else {
            portal->getClientManager()
                ->sendPacketExcept(packet, event->getType());
        }
        delete packet;
        
        break;
    }
    case Event::EventType::CREATE_OBJECT:
    case Event::EventType::SETUP_CHAT:
    case Event::EventType::SETUP_PLAYER_SETTINGS:
    case Event::EventType::GAME_STAGE_CHANGED:
    case Event::EventType::REPLACE_WORLD_SETUP:
    case Event::EventType::RACE_FINISHED:
    {
        Network::Packet *packet = new Network::EventPacket(event);
        portal->getClientManager()->sendPacket(packet);
        delete packet;
        
        break;
    }
    default:
        /*LOG2(NETWORK, PACKET, "ServerEventPropagator: Not propagating "
            << typeid(*event).name());*/
        break;
    }
}

bool ServerNetworkPortal::EventPropagator::interestedIn(
    Event::EventType::type_t type) {
    
    return true;
}

}  // namespace Server
}  // namespace Project
