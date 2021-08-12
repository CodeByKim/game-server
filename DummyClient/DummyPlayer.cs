﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class DummyPlayer
{
    private Connector mConnector;
    private int mID;

    public DummyPlayer()
    {
        mConnector = new Connector("127.0.0.1", 6000);

        mConnector.RegisterOnConnect(() =>
        {
            NetPacket packet = NetPacket.Alloc();
            short protocol = Protocol.PACKET_CS_CREATE_MY_PLAYER;
            packet.Push(protocol);
           
            mConnector.SendPacket(packet);
        });

        mConnector.RegisterOnReceive((NetPacket packet) =>
        {
            short protocol;
            packet.Pop(out protocol);

            OnPacketReceive(protocol, packet);
        });
    }

    public void Update()
    {

    }

    public void Connect()
    {
        mConnector.Connect();
    }

    private void OnPacketReceive(short protocol, NetPacket packet)
    {
        switch (protocol)
        {
            case Protocol.PACKET_SC_CREATE_MY_PLAYER:
                PacketCreateMyPlayer(packet);
                break;

            //case Protocol.PACKET_SC_CREATE_OTHER_PLAYER:
            //    PacketCreateOtherPlayer(packet);
            //    break;

            //case Protocol.PACKET_SC_DELETE_OTHER_PLAYER:
            //    PacketDeleteOtherPlayer(packet);
            //    break;

            //case Protocol.PACKET_SC_PLAYER_MOVE_START:
            //    PacketPlayerMoveStart(packet);
            //    break;

            //case Protocol.PACKET_SC_PLAYER_MOVE_END:
            //    PacketPlayerMoveEnd(packet);
            //    break;
        }
    }

    private void PacketCreateMyPlayer(NetPacket packet)
    {
        int id;
        byte dir;
        float x;
        float z;
        packet.Pop(out id).Pop(out dir).Pop(out x).Pop(out z);

        mID = id;

        Console.WriteLine("Create Player ID : " + mID);
    }
}
