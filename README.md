# 2D MMORPG GameServer

## ����
MMORPG ����� �����ϴ� ���� ���� �����ӿ�ũ�Դϴ�.
��Ƽ������ ������ NetServer, �̱۽����� ������ GameServer�� �����ϸ� 
World, Player�� ���ӿ��� ���Ǵ� ������ ����Ͽ� ���� ������ ���� �� �ֽ��ϴ�.

<img src="readme/server.png" width="700">  


<img src="readme/diagram.png" width="700">


## ����
* ���� ���
* ��� ����
* NetServer
* GameServer
* ��Ŷ �� ���� ���
* World
* Player

## ���� ���
* NetServer : �Ϲ����� ��Ƽ������ ���� ���� (�α���, ä��, �κ� ��)
* GameServer : �̱۽����� ���� ���� ����
* World : ���� ó��
* Player : ���� �÷��̾�
* Logger : ��Ƽ������ Logger
* JobManager : �񵿱� �� �ý���
* NetPacket : ��Ŷ �ø��������, ��ø��������

## ��� ����
### NetServer
```C++
#include <NetworkLib.h>

class TestMessageHandler : garam::net::IMessageHandler
{
public:
    void OnClientJoin(garam::net::ClientInfo* client) override;
    void OnClientLeave(garam::net::ClientInfo* client) override;
    void OnPacketReceive(garam::net::ClientInfo* client, garam::net::NetPacket* packet) override;
    void OnUpdate(float deltaTime) override;
};

int main()
{
    short port = 6000;
    int ccu = 4000;

    TestMessageHandler* handler = new TestMessageHandler();
    garam::net::NetServer server(port, ccu, handler);

    server.Run();
}
```
IMessageHandler �������̽��� �����Ͽ� NetServer �����ڿ� �־��ݴϴ�.
������ ó���� ��Ʈ��ũ �̺�Ʈ���� ����ڰ� ������ MessageHandler�� �Լ��� ȣ���մϴ�.

### GameServer
```C++
#include <NetworkLib.h>

class TestGameLogic : public garam::net::GameLogic
{
public:
    TestGameLogic();
    ~TestGameLogic();

    void OnUpdate(float deltaTime) override;
};

class TestMessageHandler : garam::net::IMessageHandler
{
public:
    void OnClientJoin(garam::net::ClientInfo* client) override;
    void OnClientLeave(garam::net::ClientInfo* client) override;
    void OnPacketReceive(garam::net::ClientInfo* client, garam::net::NetPacket* packet) override;
    void OnUpdate(float deltaTime) override;
};

int main()
{
    short port = 6000;
    int ccu = 4000;

    TestGameLogic* logic = new TestGameLogic();
    TestMessageHandler* handler = new TestMessageHandler();
    garam::net::GameServer server(port, ccu, handler, logic);

    server.Run();
}
```

## ��Ŷ �� ����
```C++
void TestMessageHandler::OnPacketReceive(garam::net::ClientInfo* client, garam::net::NetPacket* packet)
{	
    // ��Ŷ ������ ����
    short data1;
    int data2;
    *packet >> data1 >> data2;

    // ��Ŷ�� �����͸� ä���ְ� ����
    garam::net::NetPacket* sendPacket = garam::net::NetPacket::Alloc();
    *packet << data1 << data2;
	
    client->SendPacket(sendPacket);	

    // ����ڰ� Alloc�Ѱ� ����ڰ� Free
    garam::net::NetPacket::Free(sendPacket);
}
```

## World
```C++
```

## Player
```C++
```