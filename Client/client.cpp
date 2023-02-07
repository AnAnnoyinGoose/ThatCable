//
// Created by aag on 2/4/23.
//

#include "headers/client.h"
#include <iostream>
#include <thread>

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"

char buffer[4096];
int bufSize = sizeof(buffer);

client::client(int port, std::string &ip) {
    PORT = port;
    SOCKET = socket(AF_INET, SOCK_STREAM, 0);
    ADDRESS.sin_family = AF_INET;
    ADDRESS.sin_port = htons(PORT);
    int status = connect(SOCKET, (sockaddr *) &ADDRESS, sizeof(ADDRESS));
    if (status == -1) {
        std::cout << RED << "[x] Error connecting to server!" << std::endl;
        exit(-1);
    }
    ADDRESS.sin_addr.s_addr = inet_addr(ip.c_str());
    std::cout << GREEN << "[*] Connected to server!" << std::endl;
}

client::~client() {
    if (SOCKET != 0) {
        close(SOCKET);
        std::cout << RED << "[-] Disconnected from server!" << std::endl;
    }
}

int client::send(std::string &message) const {
    // check for empty string
    if (!message.empty()) {
        size_t len = message.size() + 1;
        ::send(SOCKET, message.c_str(), len, 0);
        return 1;
    }
    return 0;
}

size_t client::receive() const {
    size_t bytes_received = recv(SOCKET, buffer, bufSize, 0);
    return bytes_received;
}

void client::messageHandler() const {
    try {
        size_t bytes_received = receive();
        if (bytes_received == 0) {
            std::cout << RED << "[x] Server disconnected!" << std::endl;
            return;
        }
        std::cout << buffer << std::endl;
    } catch (std::exception &e) {
        std::cout << RED << "[x] Error receiving message!" << std::endl;
        return;
    }
}

void client::closeClient() const {
    std::string disconnect = "exit";
    if (send(disconnect) == 1) {
        close(SOCKET);
        std::cout << RED << "[-] Disconnected from server!" << std::endl;
    } else {
        std::cerr << "Error while sending data. " << std::endl;
    }
    _exit(-1);
}

void client::messageSender() const {
    while (true) {
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


