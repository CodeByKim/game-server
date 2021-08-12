using System.Collections;
using System.Collections.Generic;

public interface IMessageHandler
{
    void OnInit();
    void OnConnect();
    void OnDisconnect();
    void OnPacketReceive(short protocol, NetPacket packet);
}