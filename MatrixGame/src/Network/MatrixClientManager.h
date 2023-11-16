#pragma once

#include "MatrixGame.h"
//#include "MatrixNetworkManager.hpp"

#include <string>

enum GameState { 
    MAIN_MENU,
    LOBBY,
    ONGOING_MULTIPLAYER,
    SPECTATOR,
};


class MatrixClientManager {
public:
    MatrixClientManager();
    
    GameState game_state;
    int my_real_side;  // side id as it is in the server
    //MatrixNetworkManager netman;

    bool is_connected = false;
    int server_port = 5000;
    std::string server_ip = "192.0.0.1";
    std::string username = "Player Gref";


    //void EnterLobby();

    //void Cycle();  // Takt

};