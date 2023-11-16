#include "enet/enet.h"
#include "MatrixServerManager.h"
#include <stupid_logger.hpp>


MatrixServerManager::MatrixServerManager() {
    this->address.host = ENET_HOST_ANY;
    this->address.port = 1234;
    this->server = enet_host_create(&address /* the address to bind the server host to */,
                                    32 /* allow up to 32 clients and/or outgoing connections */,
                                    2 /* allow up to 2 channels to be used, 0 and 1 */,
                                    0 /* assume any amount of incoming bandwidth */,
                                    0 /* assume any amount of outgoing bandwidth */);
     if (this->server == NULL) {
        lgr.error("An error occurred while trying to create an ENet server.");
        exit(EXIT_FAILURE);
    }
}

MatrixServerManager::~MatrixServerManager() {
    enet_host_destroy(this->server);
}


MatrixServerManager *MatrixServerManager::GetInstance() {
    static MatrixServerManager instance;
    return &instance;
}

void MatrixServerManager::Loop() {
    ENetEvent event;
    /* Wait up to 1000 milliseconds for an event. */
    while (enet_host_service(this->server, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                lgr.info("A new client connected from " + std::to_string(event.peer->address.host));
                /* Store any relevant client information here. */
                //event.peer->data = "Client information";
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                //printf("A packet of length %u containing %s was received from %s on channel %u.\n",
                //       event.packet->dataLength, event.packet->data, event.peer->data, event.channelID);
                ///* Clean up the packet now that we're done using it. */
                enet_packet_destroy(event.packet);

                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                //printf("%s disconnected.\n", event.peer->data);
                ///* Reset the peer's client information. */
                event.peer->data = NULL;
        }
    }
}
