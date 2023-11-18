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
    std::unordered_map<unsigned int, RobotSnapshot> robots_at_end; // TODO: encapsulate server ids
    std::unordered_map<unsigned int, RobotSnapshot> robots_at_start;

    std::unordered_map<unsigned int, CMatrixRobotAI *> registered_robots; // get robots by their id (local)
    std::unordered_map<unsigned int, unsigned int> server_client_accordance; // robot id from the server to robot id in client
    std::unordered_map<unsigned int, unsigned int> client_server_accordance; // robot id from the client to robot id in server

    std::unordered_map<unsigned int, DeathSnapshot> death_list; // Dead soon

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

