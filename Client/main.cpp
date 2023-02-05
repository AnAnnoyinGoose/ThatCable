//
// Created by aag on 2/4/23.
//

#include "headers/main.h"
#include "headers/client.h"
#include "headers/GUI.h"
#include <iostream>
#include <string>
#include <thread>


std::string ip = "127.0.0.1";
int port = 5001;
int sckt;
client client1(port, ip);

int main() {
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
