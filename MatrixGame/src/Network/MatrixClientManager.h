#pragma once

#include "Network/MatrixNetworkManager.h"
#include "enet/enet.h"

#include <unordered_map>

class MatrixClientManager {
private:
    MatrixClientManager();

public:
    ~MatrixClientManager();

    ENetAddress address;
    ENetHost *client_peer;
    ENetPeer *server_peer;

    int start_snapshot_time = 0;
    int end_snapshot_time = 0;
    std::unordered_map<unsigned long, RobotSnapshot> robots_at_end;
    std::unordered_map<unsigned long, RobotSnapshot> robots_at_start;

    std::unordered_map < unsigned long, CMatrixRobotAI*> registered_robots;

    // Also Initializes manager if it hasn't before.
    // Make sure everything else is ready.
    static MatrixClientManager *GetInstance();

    void Loop();

    void TryConnect();
    void Say();

    void UpdateRobots(unsigned char *array, int len);
};

enum GameState { 
    MAIN_MENU,
    LOBBY,
    ONGOING_MULTIPLAYER,
    SPECTATOR,
};

