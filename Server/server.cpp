//
// Created by aag on 2/4/23.
//

#include "headers/server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <thread>

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define reset "\033[0m"
#define CONSOLE_SYMBOL "[*] "
#define ERROR_SYMBOL "[x] "
#define BUF 4096


Server::Server(int port, std::string &ip, int maxClients) {
    PORT = port;
    IP = ip;
    clients = new Client[maxClients];
    SOCKET = socket(AF_INET, SOCK_STREAM, 0);
    ADDRESS.sin_family = AF_INET;
    ADDRESS.sin_port = htons(PORT);
    ADDRESS.sin_addr.s_addr = inet_addr(IP.c_str());
    int status = bind(SOCKET, (sockaddr *) &ADDRESS, sizeof(ADDRESS));
    if (status == -1) {
        std::cout << RED << ERROR_SYMBOL << "Error binding socket!" << reset << std::endl;
        exit(-1);
    }
    status = listen(SOCKET, SOMAXCONN);
    if (status == -1) {
        std::cout << RED << ERROR_SYMBOL << "Error listening!" << reset << std::endl;
        exit(-1);
    }
    std::cout << GREEN << CONSOLE_SYMBOL << "Server started!" << reset << std::endl;
}

int Server::recieveFromClient(Server::Client &client) {
    size_t bytes_received = recv(client.SOCKET, client.buffer, BUF, 0);
    if (bytes_received == 0) {
        std::cout << RED << ERROR_SYMBOL << "Client disconnected!" << reset << std::endl;
        return -1;
    }
    return 0;
}

int Server::clientHandler(Client &client) const {
    sockaddr_in clientAddress{};
    socklen_t clientSize = sizeof(clientAddress);
    int clientSocket = accept(SOCKET, (sockaddr *) &clientAddress, &clientSize);
    if (clientSocket == -1) {
        std::cout << RED << ERROR_SYMBOL << "Error accepting connection!" << reset << std::endl;
        return -1;
    }
    std::string message = "CNCT?";
    size_t len = message.size() + 1;
    ::send(clientSocket, message.c_str(), len, 0);
    char buffer[BUF];
    size_t bytes_received = recv(clientSocket, buffer, BUF, 0);
    if (bytes_received == 0) {
        std::cout << RED << ERROR_SYMBOL << "Client disconnected!" << reset << std::endl;
        return -1;
    }
    if (std::string(buffer) == "CNCT") {
        std::cout << GREEN << CONSOLE_SYMBOL << "Client connected!" << reset << std::endl;
        client.SOCKET = clientSocket;
        client.ADDRESS = clientAddress;
        message = "IDNF!";
        len = message.size() + 1;
        ::send(clientSocket, message.c_str(), len, 0);
        bytes_received = recv(clientSocket, buffer, BUF, 0);
        if (bytes_received == 0) {
            std::cout << RED << ERROR_SYMBOL << "Client disconnected!" << reset << std::endl;
            return -1;
        }
        client.name = std::string(buffer);
        // client [index] connected with name [name] and socket [socket] and address [address]
        std::cout << GREEN << CONSOLE_SYMBOL << "Client " << client.name << " connected with socket " << clientSocket
                  << " and address " << clientAddress.sin_addr.s_addr << reset << std::endl;
        return 0;
    } else {
        std::cout << RED << ERROR_SYMBOL << "Client rejected! Received: " << std::string(buffer) << reset << std::endl;
        // close connection
        close(clientSocket);
        return -1;
    }
}

int Server::messageHandler(Server::Client &client) const {
    size_t bytes_received = recv(client.SOCKET, client.buffer, BUF, 0);
    if (bytes_received == 0) {
        std::cout << RED << ERROR_SYMBOL << "Client disconnected!" << reset << std::endl;
        return -1;
    }
    if (std::string(client.buffer) == "<!CL>>CNN<<!>") {
        std::cout << RED << ERROR_SYMBOL << "Client " << client.name << " disconnected!" << reset << std::endl;
        close(client.SOCKET);
        return -1;
    } else if (std::string(client.buffer).substr(0, 3) == "!@{") {
        std::string name = std::string(client.buffer).substr(3, std::string(client.buffer).find("}") - 3);
        std::string message =
                client.name + ": " + std::string(client.buffer).substr(std::string(client.buffer).find("}") + 1);
        size_t len = message.size() + 1;
        for (int i = 0; i < sizeof(clients); i++) {
            if (clients[i].SOCKET != 0 && clients[i].name == name) {
                ::send(clients[i].SOCKET, message.c_str(), len, 0);
            }
        }
        return 0;
    } else {
        std::string message = client.name + ": " + std::string(client.buffer);
        size_t len = message.size() + 1;
        for (int i = 0; i < sizeof(clients); i++) {
            if (clients[i].SOCKET != 0) {
                ::send(clients[i].SOCKET, message.c_str(), len, 0);
            }
        }
        return 0;
    }
}

void Server::closeServer() const {
    close(SOCKET);
    std::cout << RED << ERROR_SYMBOL << "Server closed!" << reset << std::endl;
}

[[noreturn]] void Server::Listen() const {
    while (true) {
        for (int i = 0; i < sizeof(clients); i++) {
            if (clients[i].SOCKET == 0) {
                if (clientHandler(clients[i]) == 0) {
                    break;
                }
            }
        }
        for (int i = 0; i < sizeof(clients); i++) {
            if (clients[i].SOCKET != 0) {
                if (recieveFromClient(clients[i]) == 0) {
                    messageHandler(clients[i]);
                }
            }
        }
    }
}

int main() {
    std::string ip = "0.0.0.0";
    int port = 5001;
    int maxClients = 10;
    Server server(port, ip, maxClients);
    std::thread listenThread(&Server::Listen, &server);
    listenThread.join();
}


