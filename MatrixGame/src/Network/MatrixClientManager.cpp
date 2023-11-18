#include "enet/enet.h"
#include "MatrixRobot.hpp"
#include "MatrixClientManager.h"
#include "MatrixNetworkManager.h"
#include "Interface/CConstructor.h"
#include <stupid_logger.hpp>
#include <unordered_map>

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

void MatrixClientManager::UpdateRobots(unsigned char *array, int len) {
    this->start_snapshot_time = this->end_snapshot_time;
    this->end_snapshot_time = g_MatrixMap->GetTime();

    this->robots_at_start = this->robots_at_end;

    for (int idx = 0; idx < len; idx++) {
        RobotSnapshot *snap;
        snap = reinterpret_cast<RobotSnapshot*>(array + idx * sizeof(RobotSnapshot));
        if (this->server_client_accordance.find(snap->id) == this->server_client_accordance.end()) {
            
            //// Create new robot with data
            //CMatrixRobotAI *new_robot = g_MatrixMap->StaticAdd<CMatrixRobotAI>();
            //new_robot->UnitInsert(0, MRT_HEAD, RUK_HEAD_DYNAMO);
            //new_robot->UnitInsert(0, MRT_ARMOR, RUK_ARMOR_FULLSTACK);
            ////new_robot->WeaponInsert(0, MRT_WEAPON, RUK_WEAPON_MISSILE, RUK_ARMOR_FULLSTACK, 0);
            ////new_robot->WeaponInsert(0, MRT_WEAPON, RUK_WEAPON_MISSILE, RUK_ARMOR_FULLSTACK, 1);
            //new_robot->UnitInsert(0, MRT_CHASSIS, RUK_CHASSIS_ANTIGRAVITY);
            //new_robot->m_ShadowType = g_Config.m_RobotShadow;
            //new_robot->m_ShadowSize = 128;

            //new_robot->m_Side = snap->side;
            ////new_robot->RobotWeaponInit();
            //new_robot->m_PosX = snap->pos_x;
            //new_robot->m_PosY = snap->pos_y;
            ////lgr.info("id {} side {} posX {} anim {} health {}")(
            ////        std::to_string(snap->id), std::to_string(snap->side), std::to_string(snap->pos_x),
            ////        std::to_string(static_cast<unsigned int>(snap->animation)), std::to_string(snap->health));
            //new_robot->CalcRobotMass();
            //new_robot->CreateTextures();
            //new_robot->JoinToGroup();

            ////new_robot->InitMaxHitpoint(10000.0);
            //new_robot->id = snap->id;

            //SETFLAG(g_MatrixMap->m_Flags, MMFLAG_SOUND_ORDER_ATTACK_DISABLE);
            //g_MatrixMap->GetPlayerSide()->PGOrderStop(
            //        g_MatrixMap->GetSideById(snap->side)->RobotToLogicGroup(new_robot));
            //RESETFLAG(g_MatrixMap->m_Flags, MMFLAG_SOUND_ORDER_ATTACK_DISABLE);

            SSpecialBot bot{};

            bot.m_Chassis.m_nKind = RUK_CHASSIS_ANTIGRAVITY;
            bot.m_Armor.m_Unit.m_nKind = RUK_ARMOR_FULLSTACK;
            bot.m_Head.m_nKind = RUK_HEAD_DYNAMO;

            bot.m_Weapon[0].m_Unit.m_nKind = RUK_WEAPON_LASER;
            bot.m_Weapon[1].m_Unit.m_nKind = RUK_WEAPON_LASER;
            bot.m_Weapon[2].m_Unit.m_nKind = RUK_WEAPON_LASER;
            bot.m_Weapon[3].m_Unit.m_nKind = RUK_WEAPON_LASER;

            D3DXVECTOR3 vec(0, 0, 20.0f);
            CMatrixRobotAI *r = bot.GetRobot(vec, snap->side);

            g_MatrixMap->AddObject(r, true);

            r->JoinToGroup();
            r->CreateTextures();
            r->InitMaxHitpoint(10000.0);

            //r->id = snap->id;

            SETFLAG(g_MatrixMap->m_Flags, MMFLAG_SOUND_ORDER_ATTACK_DISABLE);
            g_MatrixMap->GetSideById(snap->side)
                    ->PGOrderStop(g_MatrixMap->GetSideById(snap->side)->RobotToLogicGroup(r));
            RESETFLAG(g_MatrixMap->m_Flags, MMFLAG_SOUND_ORDER_ATTACK_DISABLE);


            this->registered_robots[r->id] = r;
            this->server_client_accordance[snap->id] = r->id;
            this->client_server_accordance[r->id] = snap->id;

            //this->robots_at_end[snap->id] = new_robot;
        } 
        //else {
        //    lgr.info("55 55 55");
        //    // Update existing robot data
        //}
        this->robots_at_end[snap->id].pos_x = snap->pos_x;
        this->robots_at_end[snap->id].pos_y = snap->pos_y;
        this->robots_at_end[snap->id].animation = snap->animation;
        //lgr.info("55 55 55");
    }
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
                    //lgr.info("Got package from server");
                    PACKET_TYPE type = static_cast<PACKET_TYPE>(event.packet->data[0]);
                    switch (type) {
                        case PACKET_TYPE::DEATH_SNAPSHOT: {
                            DeathSnapshot *ds = reinterpret_cast<DeathSnapshot *>(event.packet->data + 1);
                            CMatrixRobotAI *r = this->registered_robots[this->server_client_accordance[ds->id]];
                            this->registered_robots.erase(r->id);
                            this->robots_at_end.erase(r->id);
                            this->robots_at_start.erase(r->id);
                            //this->server_client_accordance.erase(ds->id);
                            this->client_server_accordance.erase(r->id);
                            r->MustDie();

                            break;
                        }
                        case PACKET_TYPE::ROBOTS_SNAPSHOT: {
                            unsigned char number = event.packet->data[1];  // TODO: potential error
                            //lgr.info("Number of robots: {}")(number);
                            this->UpdateRobots(&event.packet->data[2], number);
                            break;
                        }
                            //ds->time
                            //this->death_list[this->server_client_accordance[ds->id]] = *ds;
                    }
                    // const char *c_str = reinterpret_cast<const char *>(event.packet->data);
                    // std::string str(c_str, event.packet->dataLength);
                }

                enet_packet_destroy(event.packet);

                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                // printf("%s disconnected.\n", event.peer->data);
                lgr.info("Disconnect");
                event.peer->data = NULL;
        }
    }
    CMatrixMapStatic *s = CMatrixMapStatic::GetFirstLogic();
    for (; s; s = s->GetNextLogic()) {
        if (!s->IsRobot())
            continue;

        //// Death processing
        //if (this->death_list.find(s->id) != this->death_list.end() && ) {
        //    s->AsRobot()->MustDie();

        //}
        if (this->registered_robots.find(s->id) != this->registered_robots.end()) {
            CMatrixRobotAI *r = this->registered_robots[s->id];
            float phase = float(g_MatrixMap->GetTime() - this->end_snapshot_time) / float(this->end_snapshot_time - this->start_snapshot_time);
            r->m_PosX = LERPFLOAT(phase, this->robots_at_start[this->client_server_accordance[r->id]].pos_x, this->robots_at_end[this->client_server_accordance[r->id]].pos_x);
            r->m_PosY = LERPFLOAT(phase, this->robots_at_start[this->client_server_accordance[r->id]].pos_y, this->robots_at_end[this->client_server_accordance[r->id]].pos_y);

            r->m_Animation = this->robots_at_start[this->client_server_accordance[r->id]].animation;
            r->RChange(MR_Matrix | MR_ShadowProjTex | MR_ShadowStencil);
            r->RNeed(MR_Matrix);
            r->JoinToGroup();
            //r->SwitchAnimation
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
