#pragma once

#include "enet/enet.h"

class MatrixClientManager {
private:
    MatrixClientManager();

public:
    ~MatrixClientManager();

    ENetAddress address;
    ENetHost *client;
    ENetPeer *server_peer;

    // Also Initializes manager if it hasn't before.
    // Make sure everything else is ready.
    static MatrixClientManager *GetInstance();

    void Loop();

    void TryConnect();
    void Say();
};

enum GameState { 
    MAIN_MENU,
    LOBBY,
    ONGOING_MULTIPLAYER,
    SPECTATOR,
};

