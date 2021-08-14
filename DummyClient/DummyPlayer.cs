using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class DummyPlayer
{
    private Connector mConnector;
    private int mID;
    private byte mDir;
    private float mX;
    private float mZ;
    
    private bool mIsMoving;
    private int delay;

    private Random mRandom;

    public DummyPlayer(string ip, int port)
    {
        mConnector = new Connector(ip, port);        
        
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

    public void Update(int deltaTime)
    {        
        if(mConnector.IsConnect())
        {
            if(!mIsMoving)
            {
                MoveStart();

                delay = mRandom.Next(1000, 2000);
            }
            else
            {
                delay -= deltaTime;

                if(delay <= 0)
                {
                    MoveStop();
                }
            }
        }        
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

        NetPacket.Free(packet);
    }

    private void MoveStart()
    {
        mDir = (byte)mRandom.Next(0, 4);
        Protocol.SEND_PLAYER_MOVE_START(mConnector, mDir, mX, mZ);
        mIsMoving = true;
    }

    private void MoveStop()
    {
        Protocol.SEND_PLAYER_MOVE_END(mConnector, mDir, mX, mZ);
        mIsMoving = false;
    }

    #region Packet Handler
    private void PacketCreateMyPlayer(NetPacket packet)
    {
        int id;
        byte dir;
        float x;
        float z;
        packet.Pop(out id).Pop(out dir).Pop(out x).Pop(out z);

        mID = id;
        mDir = dir;
        mX = x;
        mZ = z;

        Console.WriteLine("Connect Player ID : " + mID);
        mRandom = new Random(id);               
    }
    #endregion
}
