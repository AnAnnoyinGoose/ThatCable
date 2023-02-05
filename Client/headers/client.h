//
// Created by aag on 2/4/23.
//

#ifndef THATCABLE_CLIENT_H
#define THATCABLE_CLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>


class client {
private:
    std::string name;
    sockaddr_in ADDRESS{};
    int PORT;
    std::string IP;
    int SOCKET;
public:
    client(int port, std::string &ip);

    ~client();

    void send(std::string &message) const;

    std::string receive() const;

    void closeClient() const;

    __attribute__((unused)) void messageHandler() const;

    __attribute__((unused)) void messageSender() const;

    int getSocket() const;

};


#endif //THATCABLE_CLIENT_H
