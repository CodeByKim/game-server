using System;
using System.Collections.Generic;
using System.Text;

class DummyClient
{
    private int mCCU;
    private List<DummyPlayer> mPlayers;
    private Timer mTimer;

    public DummyClient(string ip, int port, int ccu)
    {        
        mCCU = ccu;

        mPlayers = new List<DummyPlayer>();
        mTimer = new Timer();

        for (int i = 0; i < mCCU; i++)
        {
            DummyPlayer player = new DummyPlayer(ip, port);
            mPlayers.Add(player);
        }

        Console.WriteLine("Create Dummy Players...");
    }

    public void Run()
    {
        for (int i = 0; i < mPlayers.Count; i++)
        {
            mPlayers[i].Connect();
        }

        while (true)
        {
            mTimer.Update();
            OnUpdate(mTimer.GetDeltaTime());

            System.Threading.Thread.Sleep(1);
        }
    }

    private void OnUpdate(int deltaTime)
    {
        for (int i = 0; i < mPlayers.Count; i++)
        {
            mPlayers[i].Update(deltaTime);
        }
    }
}
