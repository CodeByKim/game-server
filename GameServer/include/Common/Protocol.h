#pragma once
#include <NetworkLib.h>

class garam::net::ClientInfo;
class World;
class BasePlayer;

#pragma region Protocol Number
#define PACKET_CS_CREATE_MY_PLAYER 0
#define PACKET_SC_CREATE_MY_PLAYER 1
#define PACKET_SC_CREATE_OTHER_PLAYER 2

#define PACKET_SC_REMOVE_OTHER_PLAYER 3

#define PACKET_CS_PLAYER_MOVE_START 4
#define PACKET_CS_PLAYER_MOVE_END 5
#define PACKET_SC_PLAYER_MOVE_START 6
#define PACKET_SC_PLAYER_MOVE_END 7

#define PACKET_SC_CREATE_MONSTER 8
#define PACKET_SC_REMOVE_MONSTER 9

#define PACKET_CS_PLAYER_ATTACK 10
#define PACKET_SC_PLAYER_ATTACK 11

#define PACKET_SC_MONSTER_HIT 12
#define PACKET_SC_MONSTER_DEAD 13

#define PACKET_SC_SYNC_POSITION 99
#define PACKET_CS_TELEPORT_PLAYER 100
#define PACKET_CS_CREATE_DUMMY_PLAYER 101
#pragma endregion

#pragma region Send Protocol Functions
void SEND_CREATE_MY_PLAYER(garam::net::ClientInfo& info, int id, BYTE dir, float x, float y);
void SEND_CREATE_OTHER_PLAYER(garam::net::ClientInfo& info, int id, BYTE dir, float x, float y);
void SEND_REMOVE_OTHER_PLAYER(garam::net::ClientInfo& info, int id);

void SEND_PLAYER_MOVE_START(garam::net::ClientInfo& info, int id, BYTE dir, float x, float y);
void SEND_PLAYER_MOVE_END(garam::net::ClientInfo& info, int id, BYTE dir, float x, float y);

void SEND_CREATE_MONSTER(garam::net::ClientInfo& info, int id, BYTE dir, float x, float y);
void SEND_REMOVE_MONSTER(garam::net::ClientInfo& info, int id);
#pragma endregion

#pragma region Broadcast Protocol Functions
void BROADCAST_CREATE_OTHER_PLAYER(garam::net::World& world, int id, BYTE dir, float x, float y, garam::net::BasePlayer* sender);
void BROADCAST_PLAYER_MOVE_START(garam::net::World& world, int id, BYTE dir, float x, float y, garam::net::BasePlayer* sender);
void BROADCAST_PLAYER_MOVE_END(garam::net::World& world, int id, BYTE dir, float x, float y, garam::net::BasePlayer* sender);
void BROADCAST_REMOVE_OTHER_PLAYER(garam::net::World& world, int id, garam::net::BasePlayer* sender);
void BROADCAST_PLAYER_ATTACK(garam::net::World& world, int id, BYTE dir, float x, float y, garam::net::BasePlayer* sender);
void BROADCAST_HIT_MONSTER(garam::net::World& world, int id, int hp, garam::net::BasePlayer* sender);
void BROADCAST_DEAD_MONSTER(garam::net::World& world, int id, garam::net::BasePlayer* sender);
void BROADCAST_SYNC_POSITION(garam::net::World& world, int id, float x, float y, garam::net::BasePlayer* sender);
#pragma endregion