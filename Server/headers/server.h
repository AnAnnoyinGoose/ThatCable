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
    struct Client {
        std::string name;
        int SOCKET;
        sockaddr_in ADDRESS;
        std::string IP;
        int PORT;
    };
    Client client[20];
    int SOCKET{};
    sockaddr_in ADDRESS{};
    int PORT{};
    std::string IP;

    __attribute__((unused)) void
    setClient(int clientSocket, sockaddr_in address, std::string &ip, int port, std::string name);

public:

    Server(int port, std::string &ip);

    ~Server();

    [[noreturn]] void listen() const;

    static void send(int clientSocket, std::string &message);

    static std::string receive(int clientSocket);

    void closeServer() const;

    static void inboxUpdate(int clientSocket);


// <editor-fold desc="TODO: Add these functions to the client class">
    __attribute__((unused)) static void closeClient(int clientSocket);

    void listClients();

    void sendAll(std::string &message);

    int getClientSocket(int clientSocket);

    __attribute__((unused)) sockaddr_in getClientAddress(int clientSocket);

    __attribute__((unused)) std::string getClientIP(int clientSocket);

    __attribute__((unused)) int getClientPort(int clientSocket);

    void commandHandler();

    __attribute__((unused)) int acceptClient();

    void sendSpecific(int clientSocket, std::string &message);
    // </editor-fold>




};


#endif //THATCABLE_SERVER_H
