#include "Server.h"
#include <ws2tcpip.h>

Client::~Client()
{
    if (CloseSocket(m_InSoket) && CloseSocket(m_OutSoket))
    {
        std::cout << "Close socket success" << std::endl;
    }
    else
    {
        std::cout << "Close socket failed" << std::endl;
    }
    WSACleanup();
}

bool Client::Init()
{
    WSADATA wsaData;

    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}

bool Client::CreateSockets()
{
    return CreateSocket(m_InSoket) && CreateSocket(m_OutSoket);
}

bool Client::BindSockets()
{
    return BindSocket(m_InSoket);
}

void Client::ListenSocket()
{
    SOCKADDR_IN senderAddr;
    int senderAddrSize = sizeof(senderAddr);
    const size_t buffLen = MAX_MESSAGE_LEN;
    char buff[buffLen];

    memset(buff, 0, buffLen);

    if (recvfrom(m_InSoket, buff, buffLen, 0, (SOCKADDR*)&senderAddr, &senderAddrSize) == SOCKET_ERROR)
    {
        std::cout << "recvfrom failed: " << WSAGetLastError() << std::endl;
        return;
    }
    else
    {
        char str_addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &senderAddr.sin_addr, str_addr, INET_ADDRSTRLEN);
        std::cout << "[" << str_addr << "] " << buff << std::endl;
    }
}

void Client::SendBroadcastMessage(std::string message)
{
    char broadcast = 1;
    setsockopt(m_OutSoket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));

    SOCKADDR_IN send_addr;
    send_addr.sin_family = AF_INET;
    send_addr.sin_port = htons(MY_PORT);
    send_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    if (sendto(m_OutSoket, message.c_str(), message.length(), 0, (SOCKADDR*)& send_addr, sizeof(send_addr)) == SOCKET_ERROR)
    {
        std::cout << "Failed to send broadcast msg" << std::endl;
    }
}

bool Client::CreateSocket(SOCKET& my_socket)
{
    my_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    return my_socket != INVALID_SOCKET;
}

bool Client::BindSocket(SOCKET& socket)
{
    SOCKADDR_IN ReceiverAddr;

    // The IPv4 family
    ReceiverAddr.sin_family = AF_INET;
    ReceiverAddr.sin_port = htons(MY_PORT);
    // From all interface (0.0.0.0)
    ReceiverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    return bind(socket, (SOCKADDR*)& ReceiverAddr, sizeof(ReceiverAddr)) != SOCKET_ERROR;
}

bool Client::CloseSocket(SOCKET& socket)
{
    if (socket == INVALID_SOCKET)
    {
        return false;
    }
    return closesocket(socket) == SOCKET_ERROR;
}
