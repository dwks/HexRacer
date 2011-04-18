#include "boost/asio.hpp"

#include "ServerMain.h"
#include "ClientManager.h"

#include "connection/Socket.h"
#include "connection/ServerManager.h"

#include "network/StringSerializer.h"
#include "network/PacketSerializer.h"
#include "network/HandshakePacket.h"
#include "network/EventPacket.h"
#include "network/PingPacket.h"

#include "event/QuitEvent.h"
#include "event/ChangeOfIntention.h"
#include "event/PaintEvent.h"
#include "event/PaintCellsChanged.h"
#include "event/CreateObject.h"
#include "event/DestroyObject.h"
#include "event/UpdateObject.h"
#include "event/UpdateWorld.h"
#include "event/EntireWorld.h"
#include "event/SetupChat.h"
#include "event/SetupPlayerSettings.h"
#include "event/SetupClientSettings.h"
#include "event/ReplaceWorldSetup.h"
#include "event/GameStageChanged.h"
#include "event/GeneralWorldSetupEvent.h"

#include "event/EventSystem.h"

#include "physics/PhysicsFactory.h"

#include "world/TimeElapsed.h"

#include "mesh/MeshGroup.h"
#include "mesh/MeshLoader.h"
#include "map/PathTracker.h"
#include "map/PathingUpdater.h"

#include "log/Logger.h"
#include "misc/Sleeper.h"

#include "config.h"
#include "settings/SettingsManager.h"

#include <signal.h>

namespace Project {
namespace Server {

void terminationHandler(int signal) {
    LOG2(GLOBAL, PROGRESS,
        "Server terminating after receiving signal " << signal);
    EMIT_EVENT(new Event::QuitEvent());
}

void ServerMain::ServerVisitor::visit(Network::HandshakePacket &packet) {
    LOG2(NETWORK, ERROR,
        "Server received HandshakePacket, this should never happen");
}

void ServerMain::ServerVisitor::visit(Network::EventPacket &packet) {
    // bootstrap into event subsystem
    EMIT_EVENT(packet.getEvent());
}

void ServerMain::ServerVisitor::visit(Network::PingPacket &packet) {
    unsigned long now = Misc::Sleeper::getTimeMilliseconds();
    
    packet.setMilliseconds(now);
    
    main->networkPortal->getClientManager()->setPingTimeout(
        main->whichSocket, now);
    main->networkPortal->getClientManager()->sendPacket(&packet);
}

void ServerMain::ServerObserver::observe(Event::EventBase *event) {
    switch(event->getType()) {
    case Event::EventType::QUIT:
        main->setQuit();
        break;
    case Event::EventType::CREATE_OBJECT: {
        Event::CreateObject *createObject
            = dynamic_cast<Event::CreateObject *>(event);
        
        if(dynamic_cast<Object::Player *>(createObject->getObject())) {
            Object::Player *player
                = dynamic_cast<Object::Player *>(createObject->getObject());
            
            if(true /*Settings::ProgramSettings::getInstance()->isServer()*/) {
                player->setPathTracker(
                    new Map::PathTracker(*main->getWorldManager()->getPathManager()));
            }
            
            main->getWorldManager()->addPlayer(player);
        }
        else {
            main->getWorldManager()->getWorld()->addObject(createObject->getObject());
        }
        break;
    }
    case Event::EventType::DESTROY_OBJECT: {
        Event::DestroyObject *destroyObject
            = dynamic_cast<Event::DestroyObject *>(event);
        
        main->getWorldManager()->getWorld()->removeObject(
            main->getWorldManager()->getWorld()->getObject(destroyObject->getID()));
        
        break;
    }
    case Event::EventType::UPDATE_OBJECT: {
        Event::UpdateObject *updateObject
            = dynamic_cast<Event::UpdateObject *>(event);
        
        Object::ObjectBase *object
            = main->getWorldManager()->getWorld()->getObject(updateObject->getID());
        
        object->getPhysicalObject()->setData(
            updateObject->getTransformation(),
            updateObject->getLinearVelocity(),
            updateObject->getAngularVelocity());
        
        break;
    }
    case Event::EventType::CHANGE_OF_INTENTION: {
        Event::ChangeOfIntention *changeOfIntention
            = dynamic_cast<Event::ChangeOfIntention *>(event);
        
        int id = changeOfIntention->getPlayer();
        Object::Player *player = main->getWorldManager()->getPlayer(id);

		if(changeOfIntention->getIntention().getPaint()) {
            EMIT_EVENT(new Event::TogglePainting(
                changeOfIntention->getPlayer(),
                Event::TogglePainting::PAINTING));
        }
        else if(changeOfIntention->getIntention().getErase()) {
            EMIT_EVENT(new Event::TogglePainting(
               changeOfIntention->getPlayer(),
                Event::TogglePainting::ERASING));
        }
        else {
            EMIT_EVENT(new Event::TogglePainting(
                changeOfIntention->getPlayer(),
                Event::TogglePainting::NOTHING));
        }
        
        player->setIntention(changeOfIntention->getIntention());
        
        break;
    }
    case Event::EventType::SETUP_CHAT: {
        Event::SetupChat *setupChat
            = dynamic_cast<Event::SetupChat *>(event);
        
        LOG(NETWORK, "Chat from " << setupChat->getClient() << ". "
            << setupChat->getName() << ": " << setupChat->getMessage());
        break;
    }
    case Event::EventType::SETUP_PLAYER_SETTINGS: {
        Event::SetupPlayerSettings *setupPlayerSettings
            = dynamic_cast<Event::SetupPlayerSettings *>(event);
        
        World::WorldSetup::getInstance()->replacePlayerSettings(
            setupPlayerSettings->getPlayerSettings());
        
        EMIT_EVENT(new Event::ReplaceWorldSetup(
            World::WorldSetup::getInstance()));
        
        break;
    }
    case Event::EventType::SETUP_CLIENT_SETTINGS: {
        Event::SetupClientSettings *setupClientSettings
            = dynamic_cast<Event::SetupClientSettings *>(event);
        
        int id = setupClientSettings->getClientSettings().getID();
        World::WorldSetup::ClientSettings *old
            = World::WorldSetup::getInstance()->getClientSettings(id);
        World::WorldSetup::ClientSettings now
            = setupClientSettings->getClientSettings();
        
        bool wasReady = false;
        if(old && old->isReadyToStart()) wasReady = true;
        
        World::WorldSetup::getInstance()->replaceClientSettings(
            setupClientSettings->getClientSettings());
        
        if(wasReady != now.isReadyToStart()) {
            std::string name = World::WorldSetup::getInstance()
                ->getPlayerSettings(id)->getName();
            
            EMIT_EVENT(new Event::SetupChat(-1, "", Misc::StreamAsString()
                << name << (now.isReadyToStart() ? " is " : " is NOT ")
                << "ready to start!"));
        }
        
        /*EMIT_EVENT(new Event::ReplaceWorldSetup(
            World::WorldSetup::getInstance()));*/
        
        break;
    }
    case Event::EventType::GENERAL_WORLD_SETUP: {
        Event::GeneralWorldSetupEvent *general
            = dynamic_cast<Event::GeneralWorldSetupEvent *>(event);
        World::WorldSetup *worldSetup = World::WorldSetup::getInstance();
        if(general->getData().substr(0, 7) == "add-ai ") {
            std::istringstream stream(general->getData().substr(7));
            int toAdd;
            stream >> toAdd;
            
            /*int colour = worldSetup->getPlayerSettings(general->getClient())
                ->getColor();*/
            
            LOG(NETWORK, "Creating another AI on team " << toAdd);
            
            World::WorldSetup::PlayerSettings ai;
            ai.setColor(toAdd);
            ai.setID(main->clientCount ++);
            ai.setName(Object::Player::getDefaultPlayerName(ai.getID()) + " [AI]");
            
            worldSetup->replacePlayerSettings(ai);
            
            EMIT_EVENT(new Event::ReplaceWorldSetup(worldSetup));
        }
        else if(general->getData().substr(0, 10) == "remove-ai ") {
            std::istringstream stream(general->getData().substr(10));
            int removeFrom;
            stream >> removeFrom;
            
            /*int colour = worldSetup->getPlayerSettings(general->getClient())
                ->getColor();*/
            
            LOG(NETWORK, "Trying to remove an AI from team " << removeFrom);
            
            std::vector<int> ids;
            worldSetup->getAllPlayerIDs(ids);
            
            for(std::vector<int>::size_type x = 0; x < ids.size(); x ++) {
                // don't remove other clients
                if(worldSetup->getClientSettings(ids[x]) != NULL) continue;
                
                if(worldSetup->getPlayerSettings(ids[x])->getColor() == removeFrom) {
                    LOG(NETWORK, "Removing " << ids[x]);
                    worldSetup->removePlayer(ids[x]);
                    break;
                }
            }
            
            EMIT_EVENT(new Event::ReplaceWorldSetup(worldSetup));
        }
        else if(general->getData() == "equalize") {
            int max = 0;
            for(int team = 0; team < Map::Teams::MAX_TEAMS; team ++) {
                int players = worldSetup->getPlayersOnTeam(team);
                if(players > max) max = players;
            }
            
            for(int team = 0; team < Map::Teams::MAX_TEAMS; team ++) {
                int players = worldSetup->getPlayersOnTeam(team);
                if(players == 0) continue;
                
                for(int p = players; p < max; p ++) {
                    LOG(NETWORK, "Equalize: creating another AI on team " << team);
                    
                    World::WorldSetup::PlayerSettings ai;
                    ai.setColor(team);
                    ai.setID(main->clientCount ++);
                    ai.setName(Object::Player::getDefaultPlayerName(
                        ai.getID()) + " [AI]");
                    
                    worldSetup->replacePlayerSettings(ai);
                }
            }
            
            EMIT_EVENT(new Event::ReplaceWorldSetup(worldSetup));
        }
        else {
            LOG2(NETWORK, WARNING,
                "Unknown GeneralWorldSetup request " << general->getData());
        }
        break;
    }
    case Event::EventType::DO_DISCONNECT: {
        //World::WorldSetup::getInstance()->removePlayer(main->whichSocket);
        main->networkPortal->getClientManager()->forceDisconnect(
            main->whichSocket);
        break;
    }
    default:
        LOG2(NETWORK, WARNING,
            "Don't know how to handle events of type " << event->getType());
        break;
    }
}

bool ServerMain::ServerObserver::interestedIn(Event::EventType::type_t type) {
    switch(type) {
    case Event::EventType::PAINT_EVENT:
    case Event::EventType::TOGGLE_PAINT:  // handled by PaintSubsystem
    case Event::EventType::PAUSE_GAME:  // handled by AccelControl
        return false;
    case Event::EventType::PHYSICS_TICK:  // don't care for the moment
        return false;
    case Event::EventType::WARP_ONTO_TRACK:  // handled by WarpDetector
    case Event::EventType::PLAYER_PROGRESS_EVENT:
    case Event::EventType::PHYSICS_COLLISION:
        return false;
    default:
        break;
    }
    
    return true;
}

ServerMain::ServerMain() : clientCount(0), visitor(this) {
    // install CTRL-C handler
#ifdef WIN32
    signal(SIGINT, terminationHandler);
    signal(SIGTERM, terminationHandler);
#else
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_handler = terminationHandler;
    action.sa_flags = SA_RESTART;
    
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
#endif
}

void ServerMain::initBasics() {
    loadingStarted = false;
    gameStarted = false;
    countdownStart = (unsigned long)-1;
    
    accelControl = boost::shared_ptr<Timing::AccelControl>(
        new Timing::AccelControl());
    
    server = boost::shared_ptr<Connection::ServerManager>(
        new Connection::ServerManager());
    clients = boost::shared_ptr<ClientManager>(new ClientManager());
    
    server->addServer(GET_SETTING("network.serverport", 1820));
    
    networkPortal = boost::shared_ptr<ServerNetworkPortal>(
        new ServerNetworkPortal(clients.get()));
    
    // this must execute before MapLoader loads the map
    meshLoader = boost::shared_ptr<Mesh::MeshLoader>(new Mesh::MeshLoader());
    
    ADD_OBSERVER(new ServerObserver(this));
    
    int aiCount = GET_SETTING("server.aicount", 0);
    for(int ai = 0; ai < aiCount; ai ++) {
        World::WorldSetup::getInstance()->addPlayerSettings(ai);
        World::WorldSetup::PlayerSettings *ps
            = World::WorldSetup::getInstance()->getPlayerSettings(ai);
        ps->setName(ps->getName() + " [AI]");
    }
    clientCount += aiCount;
}

void ServerMain::startGame() {
    initMap();
    initAI();
    loadingStarted = true;
    
    networkPortal->getClientManager()->setAllowableTimeout(30000);
    
    EMIT_EVENT(new Event::GameStageChanged(
        Project::World::WorldSetup::START_LOADING));
    
    sendWorldToPlayers();
}

void ServerMain::initMap() {
    //Instantiate the map
    map = boost::shared_ptr<Map::HRMap>(new Map::HRMap());
    if (map->loadMapFile(GET_SETTING("map", "maps/testtrack.hrm"))) {
        LOG(WORLD, "Loaded Map File " << GET_SETTING("map", "data/testtrack.hrm"));
    }
    else {
        LOG(WORLD, "Unable to load map " << GET_SETTING("map", "data/testtrack.hrm"));
    }
    
    basicWorld = boost::shared_ptr<World::BasicWorld>(new World::BasicWorld());
    basicWorld->constructBeforeConnect();
    basicWorld->constructServerSkippingConnect();

	paintManager = boost::shared_ptr<Paint::PaintManager>(
        new Paint::PaintManager(false));

	bonusManager = boost::shared_ptr<Bonus::GlobalBonusManager>(new Bonus::GlobalBonusManager);
    
	mapLoader = boost::shared_ptr<Map::MapLoader>(new Map::MapLoader());
	mapLoader->load(map.get(), NULL, NULL, paintManager.get());
    basicWorld->constructAfterConnect(map.get());
    
    paintSubsystem = boost::shared_ptr<Paint::PaintSubsystem>(
        new Paint::PaintSubsystem(
            basicWorld->getWorldManager(), paintManager.get(), 20));
}

void ServerMain::initAI() {
    aiManager = boost::shared_ptr<AI::AIManager>(
        new AI::AIManager(
            basicWorld->getRaceManager(),
            basicWorld->getPathManager(),
            basicWorld->getPlayerManager(),
			paintManager.get()));
    
    //int aiCount = GET_SETTING("server.aicount", 0);
    clientCount += aiManager->createAIs(World::WorldSetup::getInstance());
}

void ServerMain::run() {
    initBasics();
    //startGame();  // !!! hack
    
    // three-second timeout before the game starts
    networkPortal->getClientManager()->setAllowableTimeout(3000);
    
    bool hadMultiplePlayers = false;
    
    unsigned long lastTime = Misc::Sleeper::getTimeMilliseconds();
    quit = false;
    while(!quit) {
        handleNewConnections();
        handleDisconnections();
        
        handleIncomingPackets();
        
        if(!gameStarted && countdownStart != (unsigned long)-1) {
            updateClients(8);
        }
        
        if(networkPortal->getClientManager()->getClientsStillConnected()) {
            hadMultiplePlayers = true;
        }
        else if(hadMultiplePlayers) {
            quit = true;
        }
        
        if(gameStarted) {
            paintSubsystem->doStep(Misc::Sleeper::getTimeMilliseconds());
            
            basicWorld->doPhysics();
            basicWorld->doAI();
			basicWorld->checkRaceProgress();
            
            updateClients(5);
            
            World::TimeElapsed::getInstance().addStartTime(
                Timing::AccelControl::getInstance()->getPauseSkip());
        }
        else {
            if(loadingStarted) {
                if(countdownStart != (unsigned long)-1) {
                    unsigned long now = Misc::Sleeper::getTimeMilliseconds();
                    
                    //LOG(WORLD, "Loading... " << now << "/" << (countdownStart));
                    
                    World::WorldSetup::GameStage oldStage = lastStage;
                    
                    if(now >= countdownStart + 4000) {
                        lastStage = World::WorldSetup::RUNNING_GAME;
                        
                        gameStarted = true;
                        World::TimeElapsed::getInstance().setStartTime(
                            Misc::Sleeper::getTimeMilliseconds());
                    }
                    else if(now >= countdownStart + 3000) {
                        lastStage = World::WorldSetup::COUNT_1;
                    }
                    else if(now >= countdownStart + 2000) {
                        lastStage = World::WorldSetup::COUNT_2;
                    }
                    else if(now >= countdownStart + 1000) {
                        lastStage = World::WorldSetup::COUNT_3;
                    }
                    
                    if(oldStage != lastStage) {
                        EMIT_EVENT(new Event::GameStageChanged(lastStage));
                    }
                }
                else if(World::WorldSetup::getInstance()->everyoneFullyLoaded()) {
                    lastStage = Project::World::WorldSetup::START_COUNTDOWN;
                    EMIT_EVENT(new Event::GameStageChanged(lastStage));
                    
                    countdownStart = Misc::Sleeper::getTimeMilliseconds();
                }
            }
            else if(World::WorldSetup::getInstance()->everyoneReadyToStart()) {
                EMIT_EVENT(new Event::SetupChat(-1, "", Misc::StreamAsString()
                    << "Starting game, please wait..."));
                
                startGame();
            }
        }
        
        lastTime = doDelay(lastTime);
        
        accelControl->clearPauseSkip();
    }
    
    LOG(GLOBAL, "All players left, server is exiting");
}

void ServerMain::sendWorldToPlayers() {
    {
        LOG(WORLD, "List of players:");
        Object::WorldManager::PlayerIteratorType it
            = basicWorld->getWorldManager()->getPlayerIterator();
        while(it.hasNext()) {
            Object::Player *player = it.next();
            LOG(WORLD, "    " << player->getID()
                << " " << player->getPlayerName());
        }
        
        Event::EntireWorld *entireWorld = new Event::EntireWorld(
            getWorldManager()->getWorld(),
            getWorldManager()->getPlayerList(),
            GET_SETTING("game.race.laps", 3));
        Network::Packet *packet = new Network::EventPacket(entireWorld);
        clients->sendPacket(packet);
        delete entireWorld;
        delete packet;
    }
    
    //int aiCount = GET_SETTING("server.aicount", 0);
    
    for(int client = 0; client < clients->getSocketCount(); client ++) {
        if(!clients->socketExists(client)) continue;
        
        int id = clients->getGameID(client);
        
        LOG(WORLD, "Creating player " << id);
        
        Math::Point location = basicWorld->getRaceManager()
            ->startingPointForPlayer(id);
        Math::Point direction = basicWorld->getRaceManager()
            ->startingPlayerDirection();
        
        World::WorldSetup::PlayerSettings *settings
            = World::WorldSetup::getInstance()->getPlayerSettings(
                id);
        
        Object::Player *player = new Object::Player(
            id, location, direction);
        if(settings) {
            player->setPlayerName(settings->getName());
            player->setTeamID(settings->getColor());
        }
        
        player->setPathTracker(new Map::PathTracker(
            *basicWorld->getPathManager()));
        
        EMIT_EVENT(new Event::CreateObject(player));
    }
}

void ServerMain::handleNewConnections() {
    for(;;) {
        Connection::Socket *socket = server->checkForConnections();
        if(!socket) break;
        
        int client = clientCount;
        clientCount ++;

        Network::PacketSerializer packetSerializer;
        Network::Packet *packet = new Network::HandshakePacket(
            client,
            GET_SETTING("map", "data/testtrack.hrm"),
            GET_SETTING("game.paint.allowerase", 1),
            GET_SETTING("game.paint.allowoverwrite", 0),
            GET_SETTING("bonus.enable", 1),
            Misc::Sleeper::getTimeMilliseconds());

		LOG2(NETWORK, CONNECT, "Serializing handshake packet");
		LOG2(NETWORK, CONNECT, packet);
        
        // !!! can use clients->sendPacketOnly() ?
        Network::StringSerializer stringSerializer(socket);
		std::string packetString = packetSerializer.packetToString(packet);

		LOG2(NETWORK, CONNECT, "Sending handshake packet");
        stringSerializer.sendString(packetString);
        delete packet;

		LOG2(NETWORK, CONNECT, "Handshake packet sent");
        
        clients->addClient(socket, client);
        
        World::WorldSetup::getInstance()->addClientSettings(client);
        World::WorldSetup::getInstance()->addPlayerSettings(client);
        
        Event::EventBase *event = new Event::ReplaceWorldSetup(
            World::WorldSetup::getInstance());
        packet = new Network::EventPacket(event);
        //clients->sendPacketOnly(packet, client);
        // send to new client specially, then to everyone else
        stringSerializer.sendString(
            packetSerializer.packetToString(packet));
        clients->sendPacketExcept(packet, client);
        delete packet;
        delete event;
    }
}

void ServerMain::handleDisconnections() {
    int disconnected, gameid;
    while((disconnected = clients->nextDisconnectedClient(gameid)) >= 0) {
        LOG2(NETWORK, CONNECT,
            "Client " << disconnected << " has disconnected "
            "(game ID " << gameid << ")");
        
        if(countdownStart == (unsigned long)-1 && !gameStarted) {
            std::string name = World::WorldSetup::getInstance()
                ->getPlayerSettings(gameid)->getName();
            
            EMIT_EVENT(new Event::SetupChat(-1, "", Misc::StreamAsString()
                << name << " has disconnected"));
            
            World::WorldSetup::getInstance()->removePlayer(gameid);
            
            EMIT_EVENT(new Event::ReplaceWorldSetup(
                World::WorldSetup::getInstance()));
        }
    }
}

void ServerMain::handleIncomingPackets() {
    Network::Packet *packet;
    while((packet = clients->nextPacket(&whichSocket))) {
        /*LOG(NETWORK, "Packet received from "
            << whichSocket << ": " << packet);*/
        
        packet->accept(visitor);
        delete packet;
    }
}

void ServerMain::updateClients(int frequency) {
    static int loops = 0;
    if(++loops == frequency) {
        loops = 0;
        
        Event::UpdateWorld *update
            = new Event::UpdateWorld(
                Misc::Sleeper::getTimeMilliseconds(),
                getWorldManager());
        Network::Packet *packet = new Network::EventPacket(update);
        clients->sendPacket(packet);
        delete packet;
        delete update;
    }
}

unsigned long ServerMain::doDelay(unsigned long lastTime) {
    unsigned long currentTime = Misc::Sleeper::getTimeMilliseconds();
    unsigned long elapsed = currentTime - lastTime;
    long tosleep = 10 - elapsed;
    if(tosleep > 0) {
        Misc::Sleeper::sleep(tosleep);
    }
    
    //while(lastTime < currentTime) lastTime += 10;
    while(lastTime < currentTime + tosleep) lastTime += 10;
    
    return lastTime;
}

}  // namespace Server
}  // namespace Project
