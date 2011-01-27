#include "ServerNetworkPortal.h"
#include "network/EventPacket.h"

#include "event/ObserverList.h"
#include "event/TogglePainting.h"

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
