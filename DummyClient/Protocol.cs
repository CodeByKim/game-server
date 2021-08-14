using System;

public static class Protocol
{
    public const short PACKET_CS_CREATE_MY_PLAYER = 0;
    public const short PACKET_SC_CREATE_MY_PLAYER = 1;
    public const short PACKET_SC_CREATE_OTHER_PLAYER = 2;

    public const short PACKET_SC_DELETE_OTHER_PLAYER = 3;

    public const short PACKET_CS_PLAYER_MOVE_START = 4;
    public const short PACKET_CS_PLAYER_MOVE_END = 5;
    public const short PACKET_SC_PLAYER_MOVE_START = 6;
    public const short PACKET_SC_PLAYER_MOVE_END = 7;

    public static void SEND_CREATE_MY_PLAYER(Connector conn)
    {
        NetPacket packet = NetPacket.Alloc();
        short protocol = Protocol.PACKET_CS_CREATE_MY_PLAYER;
        packet.Push(protocol);

        conn.SendPacket(packet);
    }

    public static void SEND_PLAYER_MOVE_START(Connector conn, byte dir, float x, float z)
    {
        NetPacket packet = NetPacket.Alloc();
        short protocol = PACKET_CS_PLAYER_MOVE_START;
        packet.Push(protocol).Push(dir).Push(x).Push(z);
        conn.SendPacket(packet);
    }

    public static void SEND_PLAYER_MOVE_END(Connector conn, byte dir, float x, float z)
    {
        NetPacket packet = NetPacket.Alloc();
        short protocol = PACKET_CS_PLAYER_MOVE_END;
        packet.Push(protocol).Push(dir).Push(x).Push(z);
        conn.SendPacket(packet);
    }
}