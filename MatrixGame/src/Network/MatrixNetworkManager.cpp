#include "MatrixGame.h"
#include "MatrixNetworkManager.h"
#include "enet/enet.h"
#include <stupid_logger.hpp>
#include <sstream>

void ENet_Init() {
    if (enet_initialize() != 0) {
        lgr.error("An error occurred while initializing ENet.");
        exit(EXIT_FAILURE);
    }
}

void ENet_Deinit() {
    enet_deinitialize();
}

std::string IntToIPAddress(unsigned int ip) {
    struct in_addr addr;
    addr.s_addr = htonl(ip);
    return inet_ntoa(addr);
}
