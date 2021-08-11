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

void BROADCAST_CREATE_OTHER_PLAYER(World& world, int id, BYTE dir, float x, float y, Player* exceptPlayer)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_CREATE_OTHER_PLAYER;	
	*packet << protocol << id << dir << x << y;
	
	world.Broadcast(packet, exceptPlayer);
	garam::net::NetPacket::Free(packet);
}

void BROADCAST_PLAYER_MOVE_START(World& world, int id, BYTE dir, float x, float y, Player* exceptPlayer)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_PLAYER_MOVE_START;
	*packet << protocol << id << dir << x << y;

	world.Broadcast(packet, exceptPlayer);
	garam::net::NetPacket::Free(packet);
}

void BROADCAST_PLAYER_MOVE_END(World& world, int id, BYTE dir, float x, float y, Player* exceptPlayer)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_PLAYER_MOVE_END;
	*packet << protocol << id << dir << x << y;

	world.Broadcast(packet, exceptPlayer);
	garam::net::NetPacket::Free(packet);
}

void BROADCAST_REMOVE_OTHER_PLAYER(World& world, int id, Player* exceptPlayer)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_REMOVE_OTHER_PLAYER;
	*packet << protocol << id;

	world.Broadcast(packet, exceptPlayer);
	garam::net::NetPacket::Free(packet);
}
