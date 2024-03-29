﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class DummyPlayer : IMessageHandler
{
    private enum State
    {
        Idle,
        Move,        
    }

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
    private State mState;
    
    private float mMoveTime;
    private float mIdleTime;
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
        mMoveSpeed = 15;
        mState = State.Idle;

        OnInit();
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

            case Protocol.PACKET_SC_SYNC_POSITION:
                PacketSyncPosition(packet);
                break;
        }
    }
    public void Connect()
    {
        mConnector.Connect();
    }

    public void OnUpdate(float deltaTime)
    {
        PollNetworkEvent();
        
        if(mIsConnect)
        {
            UpdateState(deltaTime);            
        }
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

    private void UpdateState(float deltaTime)
    {
        switch (mState)
        {
            case State.Idle:
                if (mIdleTime <= 0)
                {                    
                    ChangeMoveState();
                }
                else
                {
                    mIdleTime -= deltaTime;
                }
                break;

            case State.Move:
                if (mMoveTime <= 0)
                {
                    ChangeIdleState();
                    Attack();
                }
                else
                {
                    Move(deltaTime);
                    mMoveTime -= deltaTime;
                }
                break;
        }
    }

    private void ChangeIdleState()
    {
        mState = State.Idle;
        mIdleTime = 2;

        MoveStop();
    }

    private void ChangeMoveState()
    {
        mState = State.Move;
        mMoveTime = (float)(mRandom.NextDouble() + 1);

        MoveStart();
    }

    private void Attack()
    {
        Protocol.SEND_PLAYER_ATTACK(mConnector, mDir, mX, mZ);
    }

    private void Move(float deltaTime)
    {
        float moveOffset = deltaTime * mMoveSpeed;

        if (mDir == MOVE_DIR_LEFT)
        {
            if (mX - moveOffset <= 0)
                return;

            mX -= moveOffset;
        }
        else if (mDir == MOVE_DIR_UP)
        {
            if (mZ + moveOffset >= 2000)
                return;

            mZ += moveOffset;
        }
        else if (mDir == MOVE_DIR_RIGHT)
        {
            if (mX + moveOffset >= 2000)
                return;

            mX += moveOffset;
        }
        else if (mDir == MOVE_DIR_DOWN)
        {
            if (mZ - moveOffset <= 0)
                return;

            mZ -= moveOffset;
        }
    }

    private void MoveStart()
    {
        mDir = (byte)mRandom.Next(0, 4);
        Protocol.SEND_PLAYER_MOVE_START(mConnector, mDir, mX, mZ);
    }

    private void MoveStop()
    {
        Protocol.SEND_PLAYER_MOVE_END(mConnector, mDir, mX, mZ);
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

    private void PacketSyncPosition(NetPacket packet)
    {
        int id;        
        float x;
        float z;
        packet.Pop(out id).Pop(out x).Pop(out z);

        if(mID == id)
        {
            mX = x;
            mZ = z;
        }
    }
    #endregion
}
