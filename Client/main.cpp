//
// Created by aag on 2/4/23.
//

#include "headers/main.h"
#include "headers/client.h"
#include "headers/GUI.h"
#include <iostream>
#include <string>
#include <thread>
#include <cstring>


std::string ip = "217.0.0.1";
int port = 5001;
int sckt;
client client1(port, ip);

int main() {
    ::send(client1.getSocket(), "CNCT", 4, 0);
    // wait 2 seconds for server to respond
    sleep(2);
//    ::send(client1.getSocket(), "Petr", 4, 0);

    sckt = client1.getSocket();
    GUI gui;
    gui.drawGUI();
    gui.controlHandler();
    return 0;
}

int main::getSocket() {
    return sckt;
}

void main::closeClient() {
    client1.closeClient();
}
