using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class DummyPlayer : IMessageHandler
{
    private const byte MOVE_DIR_LEFT = 0;
    private const byte MOVE_DIR_UP = 1;
    private const byte MOVE_DIR_RIGHT = 2;
    private const byte MOVE_DIR_DOWN = 3;

    private Connector mConnector;
    private int mID;
    private byte mDir;
    private float mX;
    private float mZ;
    private float mMoveSpeed;

    private bool mIsMoving;
    private float mDelay;
    private bool mIsConnect;

    private Random mRandom;
    private Object mLock;
    private Queue<NetPacket> mRecvPacketQueue;
    private Queue<NetPacket> mDispatchPacketQueue;

    public DummyPlayer(string ip, int port)
    {
        mConnector = new Connector(ip, port);
        mLock = new object();
        mRecvPacketQueue = new Queue<NetPacket>();
        mDispatchPacketQueue = new Queue<NetPacket>();
        mMoveSpeed = 20;

        OnInit();
    }

    public void OnUpdate(float deltaTime)
    {
        PollNetworkEvent();
        
        if(mIsConnect)
        {
            if(!mIsMoving)
            {
                MoveStart();
                
                mDelay = (float)(mRandom.NextDouble() + 1);
            }
            else
            {                                
                mDelay -= deltaTime;

                if(mDelay <= 0)
                {
                    MoveStop();
                }
            }

            if(mIsMoving)
            {
                Move(deltaTime);                
            }
        }
    }

    public void Connect()
    {
        mConnector.Connect();
    }

    private void RegisterCallback()
    {
        mConnector.RegisterOnConnect(() =>
        {
            NetPacket packet = NetPacket.Alloc(null);
            packet.Type = NetPacket.PacketType.Connect;

            lock (mLock)
            {
                mRecvPacketQueue.Enqueue(packet);
            }
        });

        mConnector.RegisterOnDisconnect(() =>
        {
            NetPacket packet = NetPacket.Alloc(null);
            packet.Type = NetPacket.PacketType.Disconnect;

            lock (mLock)
            {
                mRecvPacketQueue.Enqueue(packet);
            }
        });

        mConnector.RegisterOnReceive((NetPacket packet) =>
        {
            lock (mLock)
            {
                mRecvPacketQueue.Enqueue(packet);
            }
        });
    }

    private void PollNetworkEvent()
    {
        lock (mLock)
        {
            if (mRecvPacketQueue.Count > 0)
            {
                Util.Swap(ref mRecvPacketQueue, ref mDispatchPacketQueue);
            }
            else
            {
                return;
            }
        }

        while (mDispatchPacketQueue.Count != 0)
        {
            NetPacket packet = mDispatchPacketQueue.Dequeue();

            switch (packet.Type)
            {
                case NetPacket.PacketType.Connect:
                    OnConnect();
                    break;

                case NetPacket.PacketType.Disconnect:
                    OnDisconnect();
                    break;

                case NetPacket.PacketType.Receive:
                    short protocol;
                    packet.Pop(out protocol);
                    
                    OnPacketReceive(protocol, packet);
                    break;
            }

            NetPacket.Free(packet);
        }
    }

    public void OnInit()
    {
        RegisterCallback();
    }

    public void OnConnect()
    {
        Protocol.SEND_CREATE_DUMMY_PLAYER(mConnector);
    }

    public void OnDisconnect()
    {
        mConnector.Close();
        mIsConnect = false;
        Console.WriteLine("Disconnect..." + mID);
    }
    
    public void OnPacketReceive(short protocol, NetPacket packet)
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

    private void Move(float deltaTime)
    {
        //Position moveOffset[] = {
        //    {-MOVE_SPEED, 0},
        //    {0 , MOVE_SPEED},
        //    {MOVE_SPEED, 0},
        //    {0, -MOVE_SPEED}
        //};

        //Position offset = moveOffset[mCurrentDir];
        //offset.Multiply(deltaTime);
        //Position newPos = mPosition + offset;

        ////맵 범위 벗어났는지 확인 필요
        //if (newPos.x <= 0 || newPos.x >= MAP_SIZE_X ||
        //    newPos.y <= 0 || newPos.y >= MAP_SIZE_Y)
        //{
        //    return;
        //}

        //mPosition += offset;

        float moveOffset = deltaTime * mMoveSpeed;

        if (mDir == MOVE_DIR_LEFT)
        {
            if (mX - moveOffset <= 0)
                return;

            mX -= deltaTime * mMoveSpeed;
        }
        else if (mDir == MOVE_DIR_UP)
        {
            if (mZ + moveOffset >= 2000)
                return;

            mZ += deltaTime * mMoveSpeed;
        }
        else if (mDir == MOVE_DIR_RIGHT)
        {
            if (mX + moveOffset >= 2000)
                return;

            mX += deltaTime * mMoveSpeed;
        }
        else if (mDir == MOVE_DIR_DOWN)
        {
            if (mZ - moveOffset <= 0)
                return;

            mZ -= deltaTime * mMoveSpeed;
        }
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

        mIsConnect = true;
    }    
    #endregion
}
