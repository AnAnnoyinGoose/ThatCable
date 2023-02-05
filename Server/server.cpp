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
#define Console "\e[1;31m[\e[0m\e[1;77m•\e[0m\e[1;31m]\e[0m\e[1;92m-> \e[0m" // output: [•]->

Server::Server(int port, std::string &ip) {
    PORT = port;
    SOCKET = socket(AF_INET, SOCK_STREAM, 0);
    ADDRESS.sin_family = AF_INET;
    ADDRESS.sin_port = htons(PORT);
    ADDRESS.sin_addr.s_addr = INADDR_ANY;
    int status = bind(SOCKET, (sockaddr *) &ADDRESS, sizeof(ADDRESS));
    if (status == -1) {
        std::cout << RED << "[x] Error binding socket to IP/port!" << std::endl;
        exit(-1);
    }
    std::cout << GREEN << "[*] Server started!" << std::endl;
}

Server::~Server() {
    close(SOCKET);
    std::cout << RED << "[-] Server closed!" << std::endl;
}

void Server::send(int clientSocket, std::string &message) {
    ::send(clientSocket, message.c_str(), message.size() + 1, 0);
}

std::string Server::receive(int clientSocket) {
    char buffer[4096];
    recv(clientSocket, buffer, 4096, 0);
    return {buffer, 0, 4096};
}

void Server::closeServer() const {
    close(SOCKET);
}

void Server::inboxUpdate(int clientSocket) {
    std::string message;
    while (true) {
        if (clientSocket == -1) {
            std::cout << RED << "[x] Error accepting client!" << std::endl;
            exit(-1);
        }
        message = receive(clientSocket);
        if (message == "exit") {
            std::cout << RED << "[x] Client disconnected!" << std::endl;
            close(clientSocket);
            exit(-1);
        }
        // decode the message from number to string
        std::cout << Console << message << std::endl;
    }
}


[[noreturn]] void Server::listen() const {
    ::listen(SOCKET, SOMAXCONN);
    sockaddr_in clientAddress{};
    socklen_t clientSize = sizeof(clientAddress);
    int clientSocket = accept(SOCKET, (sockaddr *) &clientAddress, &clientSize);
    if (clientSocket == -1) {
        std::cout << RED << "[x] Error accepting client!" << std::endl;
        exit(-1);
    }
    std::cout << GREEN << "[*] Client connected! {" << inet_ntoa(clientAddress.sin_addr) << ":"
              << ntohs(clientAddress.sin_port) << "}" << std::endl
              << "Listening for messages..." << std::endl;
    // for each client, create a thread
    std::thread inboxUpdateThread(&Server::inboxUpdate, clientSocket);
    inboxUpdateThread.detach();
    std::string message;
    while (true) {
        std::getline(std::cin, message);
        send(clientSocket, message);
    }
}


int main() {
    std::string ip = "0.0.0.0";
    int port = 5001;
    Server server(port, ip);
    server.listen();
    return 0;
}
