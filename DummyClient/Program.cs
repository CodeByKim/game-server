using System;
using System.Collections.Generic;
using System.Text;

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
}

class Program
{
    static void Main(string[] args)
    {        
        List<DummyPlayer> players = new List<DummyPlayer>();
        for(int i = 0; i < 1500; i++)
        {
            DummyPlayer player = new DummyPlayer();
            players.Add(player);
        }

        Console.WriteLine("Create Dummy Players...");

        for (int i = 0; i < 1500; i++)
        {
            players[i].Connect();
        }

        Console.ReadLine();
    }
}
