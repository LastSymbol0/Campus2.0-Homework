#include "Server.h"

#include <future>
#include <iostream>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

bool Init(Client* client)
{
    std::cout << "Initializing..." << std::endl;

    if (client->Init())
    {
        std::cout << "WSAStartup success!" << std::endl;
    }
    else
    {
        std::cout << "WSAStartup failed!" << std::endl;
        return false;
    }

    if (client->CreateSockets())
    {
        std::cout << "Creating sockets success!" << std::endl;
    }
    else
    {
        std::cout << "Creating sockets failed!" << std::endl;
        return false;
    }

    if (client->BindSockets())
    {
        std::cout << "Binding sockets success!" << std::endl;
    }
    else
    {
        std::cout << "Binding sockets failed!" << std::endl;
        return false;
    }
    return true;
}

void ListenNewMessages(Client* client)
{
    while (true)
    {
        client->ListenSocket();
    }
}

void SendNewMessages(Client* client)
{
    while (true)
    {
        std::string message;
        std::getline(std::cin, message);
        if (message.length() > MAX_MESSAGE_LEN)
        {
            std::cout << "Too long message!" << std::endl;
        }
        client->SendBroadcastMessage(message);
    }
}

int main()
{
    Client* client = new Client();

    if (Init(client))
    {
        std::cout << "Init success!" << std::endl;
    }
    else
    {
        std::cout << "Init failed!" << std::endl;
        delete client;
        return 0;
    }

    std::cout << "Now you can send messages to other users" << std::endl << std::endl;

    auto m_ListenThread = std::async(std::launch::async, [client]() { ListenNewMessages(client); });
    auto m_SendThread = std::async(std::launch::async, [client]() { SendNewMessages(client); });

    while (true)
    {
        m_ListenThread.wait();
        m_SendThread.wait();
    }

    delete client;
    return 0;
}
