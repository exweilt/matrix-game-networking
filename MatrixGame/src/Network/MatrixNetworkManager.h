#pragma once

#include <queue>
#include <thread>
#include <map>
#include <functional>
#include <vector>
#include <future>


#if defined(CLIENT_ON) && defined(SERVER_ON)
	#error CLIENT_ON and SERVER_ON variables cannot be defined at the same time
#endif

enum GAME_NETWORK_MODE {
	SINGLEPLAYER, // Original
	CLIENT, // Client for connecting to the server
	SERVER,
};

// Used to answer to requests
enum StatusCode {
    SUCCESS,
    DENIED,
    TIMEOUT,
};

enum RequestType {
    NONE,
    CONNECT,
    DISCONNECT,
    PING, // ping request
    PONG, // answer ping
    SAY,  // chat  
    MOVE, // send robot/robots somewhere
    SNAPSHOT, // server state(robot positions and other) update info
};

enum SnapshotType {
    SNAP_RESOURCES,
    SNAP_ROBOTS,
    SNAP_MESSAGES,
    SNAP_ATTACKS, // when robots fired
    SNAP_SIDES, // who is in game, who won etc
};

struct Request {
    char sender_ip[16];
    char sender_username[32];
    tm timestamp;
    RequestType type;
    char message[1024];

    Request(RequestType typ, char *str, char sender[32]) : type(typ), sender_ip("127.0.0.1") {
        time_t tmptime = time(0);
        gmtime_s(&timestamp, &tmptime);

        memset(message, 0, sizeof(message));
        if (str != NULL) {
            int i = 0;
            for (i = 0; str[i] != '\0' && i < sizeof(message) - 1; ++i)
                ;
            memcpy(message, str, i);
        }

        memcpy(sender_username, sender, 32);
        // std::cout << message << std::endl;
    }
};

class MatrixNetworkManager {
public:
    int port;
    std::queue<Request *> request_queue;
    std::thread listening_thread;

    // if func returns false it is removed from the vector
    // std::map<RequestType, std::vector<std::function<bool(Request)>>> listening_events;

    // void make_connection(ClientManager cli, void )
    StatusCode send_request(Request *req);
    // void MakeRequest(Request* req) {

    //}

    // await_for
    void ListeningThreadCycle();
    void createPortListeningThread();
    MatrixNetworkManager() : port(5000), request_queue() /*, listeningEvents()*/ { /*createPortListeningThread();*/
    }
};