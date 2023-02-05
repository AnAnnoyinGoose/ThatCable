//
// Created by aag on 2/4/23.
//

#include "headers/client.h"
#include <iostream>
#include <thread>

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"

client::client(int port, std::string &ip) {
    PORT = port;
    SOCKET = socket(AF_INET, SOCK_STREAM, 0);
    ADDRESS.sin_family = AF_INET;
    ADDRESS.sin_port = htons(PORT);
    ADDRESS.sin_addr.s_addr = inet_addr(ip.c_str());
    int status = connect(SOCKET, (sockaddr *) &ADDRESS, sizeof(ADDRESS));
    if (status == -1) {
        std::cout << RED << "[x] Error connecting to server!" << std::endl;
        exit(-1);
    }
    std::cout << GREEN << "[*] Connected to server!" << std::endl;
}

client::~client() {
    close(SOCKET);
    std::cout << RED << "[-] Disconnected from server!" << std::endl;
}

void client::send(std::string &message) const {
    ::send(SOCKET, message.c_str(), message.size() + 1, 0);
}

std::string client::receive() const {
    char buffer[4096];
    recv(SOCKET, buffer, 4096, 0);
    return {buffer, 0, 4096};
}

void client::closeClient() const {
    std::string disconnect = "exit";
    send(disconnect);
    close(SOCKET);
    std::cout << RED << "[-] Disconnected from server!" << std::endl;
    exit(-1);
}

__attribute__((unused)) void client::messageHandler() const {
    // update the reciever
    while (true) {
        std::string message = receive() + "\n";
        std::cout << message << std::endl;
    }
}

__attribute__((unused)) void client::messageSender() const {
    // update the sender
    while (true) {
        // make sure that if a crash occurs, the client will be disconnected
        try {
            std::string message;
            std::getline(std::cin, message);
            if (message == "exit") {
                break;
                closeClient();
            }
            send(message);
        } catch (std::exception &e) {
            std::cout << RED << "[x] Error sending message!" << std::endl;
            exit(-1);
        }
    }
}

int client::getSocket() const {
    return SOCKET;
}




// getters


