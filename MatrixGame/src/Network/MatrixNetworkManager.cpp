#include "MatrixGame.h"
#include "MatrixNetworkManager.h"
#include "enet/enet.h"
#include <stupid_logger.hpp>
#include <bitset>

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
    std::bitset<32> bits(ip);
    std::string ipString;
    for (size_t i = 0; i < 32; i += 8) {
        if (!ipString.empty())
            ipString += '.';
        ipString += std::to_string((bits >> i & std::bitset<32>(255)).to_ulong());
    }
    return ipString;
}
