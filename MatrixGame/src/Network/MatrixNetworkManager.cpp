#include "MatrixGame.h"
#include "MatrixNetworkManager.h"
#include "enet/enet.h"
#include <stupid_logger.hpp>

void ENet_Init() {
    if (enet_initialize() != 0) {
        lgr.error("An error occurred while initializing ENet.");
        exit(EXIT_FAILURE);
    }
}

void ENet_Deinit() {
    enet_deinitialize();
}
