#include "ServerNetworkPortal.h"
#include "network/EventPacket.h"

#include "event/ObserverList.h"
#include "event/PaintEvent.h"

#include "log/Logger.h"

namespace Project {
namespace Server {

ServerNetworkPortal::ServerNetworkPortal(ClientManager *clientManager)
    : clientManager(clientManager) {
    
    ADD_OBSERVER(new EventPropagator(this));
}

void ServerNetworkPortal::EventPropagator::observe(Event::EventBase *event) {
    switch(event->getType()) {
    case Event::EventType::PAINT_CELLS_CHANGED:
    {
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
