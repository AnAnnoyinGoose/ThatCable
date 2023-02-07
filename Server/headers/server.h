//
// Created by aag on 2/4/23.
//

#ifndef THATCABLE_SERVER_H
#define THATCABLE_SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>

class Server {
private:
    int SOCKET{};
    sockaddr_in ADDRESS{};
    int PORT{};
    std::string IP;
public:
    struct Client {
        sockaddr_in ADDRESS{};
        int SOCKET{};
        std::string name;
        char buffer[4096]{};
    };
    Client *clients{};

    Server(int port, std::string &ip, int maxClients);

    int clientHandler(Client &client) const;

    int messageHandler(Client &client) const;

    static int recieveFromClient(Client &client);

    void closeServer() const;

    [[noreturn]] void Listen() const;

};


#endif //THATCABLE_SERVER_H
