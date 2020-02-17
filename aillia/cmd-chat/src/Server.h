#pragma once

#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")// Link with ws2_32.lib

#define MY_PORT 42114

const size_t MAX_MESSAGE_LEN = 512;

class Client
{
public:
    ~Client();

    bool Init();

    bool CreateSockets();

    bool BindSockets();

    void ListenSocket();

    void SendBroadcastMessage(std::string message);

private:
    bool CreateSocket(SOCKET& socket);
    bool BindSocket(SOCKET& socket);
    bool CloseSocket(SOCKET& socket);

    SOCKET m_InSoket = INVALID_SOCKET;
    SOCKET m_OutSoket = INVALID_SOCKET;
};

