#include "Common/Protocol.h"
#include "Contents/World.h"

void SEND_CREATE_MY_PLAYER(garam::net::ClientInfo& info, int id, BYTE dir, float x, float y)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_CREATE_MY_PLAYER;
	*packet << protocol << id << dir << x << y;

	info.SendPacket(packet);
	garam::net::NetPacket::Free(packet);
}

void SEND_CREATE_OTHER_PLAYER(garam::net::ClientInfo& info, int id, BYTE dir, float x, float y)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_CREATE_OTHER_PLAYER;
	*packet << protocol << id << dir << x << y;

	info.SendPacket(packet);
	garam::net::NetPacket::Free(packet);
}

void SEND_REMOVE_OTHER_PLAYER(garam::net::ClientInfo& info, int id)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_REMOVE_OTHER_PLAYER;
	*packet << protocol << id;

	info.SendPacket(packet);
	garam::net::NetPacket::Free(packet);
}

void SEND_PLAYER_MOVE_START(garam::net::ClientInfo& info, int id, BYTE dir, float x, float y)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_PLAYER_MOVE_START;
	*packet << protocol << id << dir << x << y;

	info.SendPacket(packet);
	garam::net::NetPacket::Free(packet);
}

void SEND_PLAYER_MOVE_END(garam::net::ClientInfo& info, int id, BYTE dir, float x, float y)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_PLAYER_MOVE_END;
	*packet << protocol << id << dir << x << y;

	info.SendPacket(packet);
	garam::net::NetPacket::Free(packet);
}

void SEND_CREATE_MONSTER(garam::net::ClientInfo& info, int id, BYTE dir, float x, float y)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_CREATE_MONSTER;
	*packet << protocol << id << dir << x << y;

	info.SendPacket(packet);
	garam::net::NetPacket::Free(packet);
}

void SEND_REMOVE_MONSTER(garam::net::ClientInfo& info, int id)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_REMOVE_MONSTER;
	*packet << protocol << id;

	info.SendPacket(packet);
	garam::net::NetPacket::Free(packet);
}

void BROADCAST_CREATE_OTHER_PLAYER(garam::net::World& world, int id, BYTE dir, float x, float y, garam::net::BasePlayer* sender)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_CREATE_OTHER_PLAYER;	
	*packet << protocol << id << dir << x << y;
	
	world.Broadcast(packet, sender);
	garam::net::NetPacket::Free(packet);
}

void BROADCAST_PLAYER_MOVE_START(garam::net::World& world, int id, BYTE dir, float x, float y, garam::net::BasePlayer* sender)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_PLAYER_MOVE_START;
	*packet << protocol << id << dir << x << y;

	world.Broadcast(packet, sender);
	garam::net::NetPacket::Free(packet);
}

void BROADCAST_PLAYER_MOVE_END(garam::net::World& world, int id, BYTE dir, float x, float y, garam::net::BasePlayer* sender)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_PLAYER_MOVE_END;
	*packet << protocol << id << dir << x << y;

	world.Broadcast(packet, sender);
	garam::net::NetPacket::Free(packet);
}

void BROADCAST_REMOVE_OTHER_PLAYER(garam::net::World& world, int id, garam::net::BasePlayer* sender)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_REMOVE_OTHER_PLAYER;
	*packet << protocol << id;

	world.Broadcast(packet, sender);
	garam::net::NetPacket::Free(packet);
}

void BROADCAST_PLAYER_ATTACK(garam::net::World& world, int id, BYTE dir, float x, float y, garam::net::BasePlayer* sender)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_PLAYER_ATTACK;
	*packet << protocol << id << dir << x << y;

	world.Broadcast(packet, sender);
	garam::net::NetPacket::Free(packet);
}

void BROADCAST_HIT_MONSTER(garam::net::World& world, int id, int hp, garam::net::BasePlayer* sender)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_MONSTER_HIT;
	*packet << protocol << id << hp;

	world.Broadcast(packet, sender, false);
	garam::net::NetPacket::Free(packet);
}

void BROADCAST_DEAD_MONSTER(garam::net::World& world, int id, garam::net::BasePlayer* sender)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_MONSTER_DEAD;
	*packet << protocol << id;

	world.Broadcast(packet, sender, false);
	garam::net::NetPacket::Free(packet);
}

void BROADCAST_SYNC_POSITION(garam::net::World& world, int id, float x, float y, garam::net::BasePlayer* sender)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_SYNC_POSITION;
	*packet << protocol << id << x << y;

	world.Broadcast(packet, sender, false);
	garam::net::NetPacket::Free(packet);
}
