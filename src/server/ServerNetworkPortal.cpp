#include "ServerNetworkPortal.h"
#include "network/EventPacket.h"

#include "event/EventSystem.h"
#include "event/TogglePainting.h"
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
    case Event::EventType::TOGGLE_PAINT:
    {
        Network::Packet *packet = new Network::EventPacket(event);
        
        Event::TogglePainting *toggle
            = dynamic_cast<Event::TogglePainting *>(event);
        LOG(NETWORK, "server: see TogglePainting " << toggle->getPropagate());
        //if(!toggle->getPropagate()) break;
        // always propagate
        toggle->setPropagate(false);
        
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
    case Event::EventType::CREATE_OBJECT: {
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
