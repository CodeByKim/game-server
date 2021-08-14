using System;
using System.Collections.Generic;
using System.Text;

class Program
{    
    static void Main(string[] args)
    {
        const int ccu = 4500;

        DummyClient client = new DummyClient(ccu);
        client.Run();
    }
}
