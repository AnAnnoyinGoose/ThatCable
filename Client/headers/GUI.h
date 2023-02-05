//
// Created by aag on 2/4/23.
//

#ifndef THATCABLE_GUI_H
#define THATCABLE_GUI_H

#include "client.h"
#include <ncurses.h>
#include <string>
#include <iostream>
#include <thread>

class GUI {
private:
    WINDOW *chatWindow;
    WINDOW *inputWindow;
    WINDOW *usersWindow;
    WINDOW *FileBrowser;
    std::string input;
    std::string chat;
    std::string users;
    bool FileAdder;
    // array of windows
    std::string windows[3] = {"chat", "input", "users"};
    int activeWindow = 0;
public:
    GUI();

    ~GUI();

    void drawGUI() const;

    void drawChat() const;

    void drawInput() const;

    void drawUsers() const;

    void highlightActiveWindow() const;

    void controlHandler();

    void InputMode();

    void FileAdderToggle();
};


#endif //THATCABLE_GUI_H
