#pragma once

#include "Network/MatrixNetworkManager.h"
#include "enet/enet.h"



class MatrixServerManager {
private:
    MatrixServerManager();
public:
    ~MatrixServerManager();

    ENetAddress address;
    ENetHost *server;
    ENetPeer *client_peer;

    // Also Initializes manager if it hasn't before.
    // Make sure everything else is ready.
    static MatrixServerManager *GetInstance();

    int tick_period = TICK_PERIOD;
    int last_tick_time = 0;
    void Loop();
    void Tick();

    void send_package(void *data, int length);
};