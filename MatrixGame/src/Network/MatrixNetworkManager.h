#pragma once

//#include "MatrixGame.h"
//#include "MatrixMap.hpp"
#include "MatrixObjectRobot.hpp"

#include "enet/enet.h"

#include <queue>
#include <thread>
#include <map>
#include <functional>
#include <vector>
#include <future>
#include <functional>



#define TICKRATE 8

// in ms
#define TICK_PERIOD int(1000 / TICKRATE)

#if defined(CLIENT_ON) && defined(SERVER_ON)
	#error CLIENT_ON and SERVER_ON variables cannot be defined at the same time
#endif

enum GAME_NETWORK_MODE {
	SINGLEPLAYER, // Original
	CLIENT, // Client for connecting to the server
	SERVER,
};

// Max value has to be below 256
enum class PACKET_TYPE {
    NONE,
    ROBOTS_SNAPSHOT,
    DEATH_SNAPSHOT,
};

struct RobotSnapshot {
    unsigned int time;
    unsigned int id;
    float pos_x;
    float pos_y;
    unsigned char side;
    int max_health;
    int health;
    EAnimation animation;
};

struct DeathSnapshot {
    int time;
    unsigned int id;
};

// Aborts if Initialization is unsuccessful
void ENet_Init();

void ENet_Deinit();

std::string IntToIPAddress(unsigned int ip);
unsigned int IPAdressToInt(std::string);



//enum RequestType {
//    NONE,
//    CONNECT,
//    DISCONNECT,
//    PING, // ping request
//    PONG, // answer ping
//    SAY,  // chat  
//    MOVE, // send robot/robots somewhere
//    SNAPSHOT, // server state(robot positions and other) update info
//};

//enum SnapshotType {
//    SNAP_RESOURCES,
//    SNAP_ROBOTS,
//    SNAP_MESSAGES,
//    SNAP_ATTACKS, // when robots fired
//    SNAP_SIDES, // who is in game, who won etc
//};

//class MatrixNetworkManager {
//public:
//    int port;
//
//    // if func returns false it is removed from the vector
//    // std::map<RequestType, std::vector<std::function<bool(Request)>>> listening_events;
//
//    // void make_connection(ClientManager cli, void )
//    // void MakeRequest(Request* req) {
//
//    //}
//
//    // await_for
//    void ListeningThreadCycle();
//    void createPortListeningThread();
//    MatrixNetworkManager() : port(5000), request_queue() /*, listeningEvents()*/ { /*createPortListeningThread();*/
//    }
//};