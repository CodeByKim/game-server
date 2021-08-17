using System;
using System.Collections.Generic;
using System.Text;

class Program
{    
    static void Main(string[] args)
    {
        const string ip = "127.0.0.1";
        const int port = 6000;
        const int ccu = 4500;

        DummyClient client = new DummyClient(ip, port, ccu);
        client.Run();
    }
}
