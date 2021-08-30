# 2D MMORPG GameServer

## 개요
MMORPG 기능을 지원하는 게임 서버 프레임워크입니다.
멀티스레드 서버인 NetServer, 싱글스레드 로직의 GameServer를 지원하며 
World, Player등 게임에서 사용되는 모듈들을 사용하여 쉽게 서버를 만들 수 있습니다.

<img src="readme/server.png" width="700">  


<img src="readme/diagram.png" width="700">


## 목차
* 지원 기능
* 사용 예제
* NetServer
* GameServer
* 패킷 송 수신 방법
* World
* Player

## 지원 기능
* NetServer : 일반적인 멀티스레드 지원 서버 (로그인, 채널, 로비 등)
* GameServer : 싱글스레드 로직 게임 서버
* World : 섹터 처리
* Player : 게임 플레이어
* Logger : 멀티스레드 Logger
* JobManager : 비동기 잡 시스템
* NetPacket : 패킷 시리얼라이즈, 디시리얼라이즈

## 사용 예제
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