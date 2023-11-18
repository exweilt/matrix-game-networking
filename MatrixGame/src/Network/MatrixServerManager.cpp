#include "enet/enet.h"
#include "MatrixMap.hpp"
#include "MatrixMapStatic.hpp"
#include "MatrixServerManager.h"
#include "MatrixNetworkManager.h"
#include "MatrixRobot.hpp"
#include "MatrixObjectRobot.hpp"
#include <stupid_logger.hpp>


MatrixServerManager::MatrixServerManager() {
    this->address.host = ENET_HOST_ANY;
    this->address.port = 1234;
    this->server = enet_host_create(&address /* the address to bind the server host to */,
                                    32 /* allow up to 32 clients and/or outgoing connections */,
                                    2 /* allow up to 2 channels to be used, 0 and 1 */,
                                    0 /* assume any amount of incoming bandwidth */,
                                    0 /* assume any amount of outgoing bandwidth */);
    if (this->server == NULL) {
        lgr.error("An error occurred while trying to create an ENet server.");
        exit(EXIT_FAILURE);
    }

    this->last_tick_time = 0;
    this->tick_period = TICK_PERIOD;
}

MatrixServerManager::~MatrixServerManager() {
    enet_host_destroy(this->server);
}


MatrixServerManager *MatrixServerManager::GetInstance() {
    static MatrixServerManager instance;
    return &instance;
}

void MatrixServerManager::send_package(void *data, int length) {
    if (!this->client_peer)
        return;
    ENetPacket *packet = enet_packet_create(data, length, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
    enet_peer_send(this->client_peer, 0, packet);
    enet_host_flush(this->server);
}

void MatrixServerManager::send_deathsnapshot(unsigned int robot_id) {
    static unsigned char buffer[sizeof(DeathSnapshot) + 1];
    buffer[0] = static_cast<unsigned char>(PACKET_TYPE::DEATH_SNAPSHOT);
    DeathSnapshot ds(g_MatrixMap->GetTime(), robot_id);

    new (buffer + 1) DeathSnapshot;

    reinterpret_cast<DeathSnapshot *>(&buffer[1])->time = g_MatrixMap->GetTime();
    reinterpret_cast<DeathSnapshot *>(&buffer[1])->id = robot_id;

    send_package(buffer, sizeof(buffer));

}

void MatrixServerManager::Loop() {
    ENetEvent event;
    while (enet_host_service(this->server, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                lgr.info("A new client connected from " + IntToIPAddress(event.peer->address.host));
                this->client_peer = event.peer;
                /* Store any relevant client information here. */
                //event.peer->data = "Client information";
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                if (event.packet && event.packet->data && event.packet->dataLength > 0) {
                    const char *c_str = reinterpret_cast<const char *>(event.packet->data);
                    std::string str(c_str, event.packet->dataLength);
                    lgr.info("Got package: " + str);
                }

                enet_packet_destroy(event.packet);

                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                lgr.info("Disconnect");
                event.peer->data = NULL;
        }
    }

    unsigned int time = g_MatrixMap->GetTime();
    if (time > this->last_tick_time + this->tick_period) {
        Tick();
        this->last_tick_time = time;
    }

}

#define MAX_ROBOTS 256                  // Max 256 due to num of type unsigned char
void MatrixServerManager::Tick() {
    if (!this->client_peer)
        return;

    // buffer used over vector or other data structures for Perfomance purposes
    // Here first byte stores Type of package second byte stores number of RobotSnapshot
    static unsigned char robots_buffer[MAX_ROBOTS * sizeof(RobotSnapshot) + 2]{};


    unsigned int time = g_MatrixMap->GetTime();

    unsigned char num = 0;
    CMatrixMapStatic *s = CMatrixMapStatic::GetFirstLogic();
    for (; s; s = s->GetNextLogic()) {
        if (s->IsRobot()) {
            if (num > MAX_ROBOTS) {
                lgr.error("Too much robots: attempt of stack overflow");
                break;
            }
            RobotSnapshot snap;
            snap.id = s->id;
            snap.side = s->GetSide();
            snap.pos_x = s->AsRobot()->m_PosX;
            snap.pos_y = s->AsRobot()->m_PosY;
            snap.time = time;
            snap.health = s->AsRobot()->GetHitPoint();
            snap.max_health = s->AsRobot()->GetMaxHitPoint();
            snap.animation = s->AsRobot()->GetAnimation();

            memcpy(&robots_buffer[2 + num * sizeof(RobotSnapshot)], &snap, sizeof(RobotSnapshot));
            num++;
        }
    }
    robots_buffer[0] = static_cast<unsigned char>(PACKET_TYPE::ROBOTS_SNAPSHOT);
    robots_buffer[1] = num;
    this->send_package(robots_buffer, num*sizeof(RobotSnapshot) + 2);
}
#undef MAX_ROBOTS