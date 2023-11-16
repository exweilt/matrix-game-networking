#pragma once

#include "enet/enet.h"

class MatrixServerManager {
private:
    MatrixServerManager();
public:
    ~MatrixServerManager();

    ENetAddress address;
    ENetHost *server;

    // Also Initializes manager if it hasn't before.
    // Make sure everything else is ready.
    static MatrixServerManager *GetInstance();

    void Loop();
};