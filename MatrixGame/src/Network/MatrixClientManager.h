#pragma once

#include "MatrixGame.h"
#include "MatrixNetworkManager.hpp"

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

    bool is_connected;
    std::string server_ip = "192.0.0.1";
    int server_port;
    std::string username;


    void make_connection();
    void on_successful_connection();
    void on_failed_connection(StatusCode code);
    void Connect();
    //void EnterLobby();

    void Cycle();  // Takt

    ClientManager();

};