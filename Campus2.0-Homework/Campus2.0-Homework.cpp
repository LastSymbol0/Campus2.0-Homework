#include "Server.h"

#include <winsock2.h>
//#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
    std::cout << "Initializing..\n";

    Server* server = new Server();

    if (server->Init())
    {
        std::cout << "WSAStartup success!" << std::endl;
    }
    else
    {
        std::cout << "WSAStartup failed!" << std::endl;
    }

    if (server->CreateSocket())
    {
        std::cout << "CreateSocket success!" << std::endl;
    }
    else
    {
        std::cout << "CreateSocket failed!" << std::endl;
    }

    if (server->BindSocket())
    {
        std::cout << "BindSocket success!" << std::endl;
    }
    else
    {
        std::cout << "BindSocket failed!" << std::endl;
    }

    while (true)
    {
        std::string input;
        std::cout << "Press 'l' for listen, or 's' for send..." << std::endl;
        std::cin >> input;
        if (input == "l")
        {
            server->ListenSocket();
        }
        else if (input == "s")
        {
            server->SendBroadcastMessage("dewd");
        }
        std::cout << std::endl;
    }
}
