#include "enet/enet.h"
#include "MatrixClientManager.h"
#include "MatrixNetworkManager.h"
#include <stupid_logger.hpp>

MatrixClientManager::MatrixClientManager() {
    this->address.host = ENET_HOST_ANY;
    this->address.port = 5500;
    this->client_peer = enet_host_create(NULL /* create a client host */, 1 /* only allow 1 outgoing connection */,
                              2 /* allow up 2 channels to be used, 0 and 1 */,
                              0 /* assume any amount of incoming bandwidth */,
                              0 /* assume any amount of outgoing bandwidth */);
    if (this->client_peer == NULL) {
        lgr.error("An error occurred while trying to create an ENet client.");
        exit(EXIT_FAILURE);
    }
}

MatrixClientManager::~MatrixClientManager() {
    enet_host_destroy(this->client_peer);
}

MatrixClientManager *MatrixClientManager::GetInstance() {
    static MatrixClientManager instance;
    return &instance;
}

void MatrixClientManager::Loop() {
    ENetEvent event;
    while (enet_host_service(this->client_peer, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                lgr.info("Connected to server: " + IntToIPAddress(event.peer->address.host));
                /* Store any relevant client information here. */
                // event.peer->data = "Client information";
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                if (event.packet && event.packet->data && event.packet->dataLength > 0) {
                    lgr.info("Got package from server");
                    PACKET_TYPE type = static_cast<PACKET_TYPE>(event.packet->data[0]);
                    switch (type) {
                        case PACKET_TYPE::ROBOTS_SNAPSHOT:
                            unsigned char number = event.packet->data[1]; // TODO: potential error
                            lgr.info("Number of robots: {}")(number);
                            break;
                    }
                    //const char *c_str = reinterpret_cast<const char *>(event.packet->data);
                    //std::string str(c_str, event.packet->dataLength);
                }

                enet_packet_destroy(event.packet);

                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                // printf("%s disconnected.\n", event.peer->data);
                lgr.info("Disconnect");
                event.peer->data = NULL;
        }
    }
}

void MatrixClientManager::TryConnect() {
    ENetAddress server_address;
    ENetEvent event;

    enet_address_set_host(&server_address, "127.0.0.1");  // Replace with your server's IP
    server_address.port = 1234;                    // Replace with your server's port

    this->server_peer = enet_host_connect(this->client_peer, &server_address, 2, 0);
    lgr.info("TryConnnect command executed");
    if (this->server_peer == NULL) {
        lgr.error("No available peers for initiating an ENet connection.");
        exit(EXIT_FAILURE);
    }
    if (enet_host_service(client_peer, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        ENetPacket *packet = enet_packet_create(0, 0, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
        enet_peer_send(this->server_peer, 0, packet);
        enet_host_flush(this->client_peer);
        lgr.info("Connected to the server.");
    }
    else {
        enet_peer_reset(this->server_peer);
        lgr.info("Connection failed.");
    }
    //enet_host_service(this->client); 
}

void MatrixClientManager::Say() {
    
    ENetPacket *packet = enet_packet_create("aboba", strlen("aboba") + 1, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
    enet_peer_send(this->server_peer, 0, packet);
    enet_host_flush(this->client_peer);
    lgr.info("sent package");
}
