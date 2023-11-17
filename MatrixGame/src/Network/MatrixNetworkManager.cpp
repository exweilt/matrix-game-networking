#include "MatrixGame.h"
#include "MatrixMap.hpp"
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

// Returns 0 if IP adress is invalid
unsigned int IPAddressToInt(const std::string &ipString) {
    std::istringstream iss(ipString);
    unsigned int ip = 0;
    unsigned int octet;
    char delimiter;

    for (int i = 0; i < 4; ++i) {
        iss >> octet;            // Read the integer value
        ip = (ip << 8) | octet;  // Shift and combine the octets
        if (i < 3) {
            iss >> delimiter;  // Read the delimiter '.'
            if (delimiter != '.') {
                std::cerr << "Invalid IP address format!" << std::endl;
                return 0;
            }
        }
    }
    return ip;
}