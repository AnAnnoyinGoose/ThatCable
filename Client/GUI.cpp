//
// Created by aag on 2/4/23.
//

#include <cstring>
#include "headers/GUI.h"
#include "headers/client.h"
#include "headers/main.h"

#define RESET "\033[0m"
// <editor-fold desc="Text Colors">
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BLACK "\033[30m"
// </editor-fold>
// <editor-fold desc="Background Colors">
#define BRED "\033[41m"
#define BGREEN "\033[42m"
#define BYELLOW "\033[43m"
#define BBLUE "\033[44m"
#define BMAGENTA "\033[45m"
#define BCYAN "\033[46m"
#define BWHITE "\033[47m"
#define BBLACK "\033[40m"
// </editor-fold>
// <editor-fold desc="Text Styles">
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"
#define BLINK "\033[5m"
#define REVERSE "\033[7m"
#define HIDDEN "\033[8m"
// </editor-fold>

// <editor-fold desc="Letters">
#define A 97
#define B 98
#define C 99
#define D 100
#define E 101
#define F 102
#define G 103
#define H 104
#define I 105
#define J 106
#define K 107
#define L 108
#define M 109
#define N 110
#define O 111
#define P 112
#define Q 113
#define R 114
#define S 115
#define T 116
#define U 117
#define V 118
#define W 119
#define X 120
#define Y 121
#define Z 122
// </editor-fold>
// <editor-fold desc="Misc">
#define ENTER 10
#define BACKSPACE 127
#define SPACE 32
#define TAB 9
#define ESC 27
// </editor-fold>
// <editor-fold desc="Modifiers">
#define CTRL 17
#define ALT 18
#define SHIFT 16
// </editor-fold>




GUI::GUI() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    refresh();
    chatWindow = newwin(LINES * 4 / 5, COLS * 2 / 3, 0, 0);
    inputWindow = newwin(LINES / 5, COLS * 3 / 3, LINES * 4 / 5, 0);
    usersWindow = newwin(LINES * 4 / 5, COLS * 1 / 3, 0, COLS * 2 / 3);
    FileBrowser = newwin(LINES * 1 / 3, COLS * 1 / 3, LINES * 1 / 3, COLS * 1 / 3);
    FileAdder = false;
}

GUI::~GUI() {
    delwin(chatWindow);
    delwin(inputWindow);
    delwin(usersWindow);
    endwin();
}

void GUI::drawGUI() const {
    drawChat();
    drawInput();
    drawUsers();
    highlightActiveWindow();

}

void GUI::drawChat() const {
    wclear(chatWindow);
    box(chatWindow, 0, 0);
    mvwprintw(chatWindow, 0, 5, "Chat");
    wattron(chatWindow, A_BOLD);

}

void GUI::drawInput() const {
    wclear(inputWindow);
    box(inputWindow, 0, 0);
    mvwprintw(inputWindow, 0, 5, "Input");
    wattron(inputWindow, A_BOLD);
    wrefresh(inputWindow);
}

void GUI::drawUsers() const {
    wclear(usersWindow);
    box(usersWindow, 0, 0);
    mvwprintw(usersWindow, 0, 5, "Users");
    wattron(usersWindow, A_BOLD);
    wrefresh(usersWindow);
}

void GUI::highlightActiveWindow() const {
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);

    if (activeWindow == 0) {
        wbkgd(chatWindow, COLOR_PAIR(1));
        wbkgd(inputWindow, COLOR_PAIR(2));
        wbkgd(usersWindow, COLOR_PAIR(2));
    } else if (activeWindow == 1) {
        wbkgd(chatWindow, COLOR_PAIR(2));
        wbkgd(inputWindow, COLOR_PAIR(1));
        wbkgd(usersWindow, COLOR_PAIR(2));
    } else if (activeWindow == 2) {
        wbkgd(chatWindow, COLOR_PAIR(2));
        wbkgd(inputWindow, COLOR_PAIR(2));
        wbkgd(usersWindow, COLOR_PAIR(1));
    }
    wrefresh(chatWindow);
    wrefresh(inputWindow);
    wrefresh(usersWindow);
}

void GUI::InputMode() {
    int ch;
    int x = 1;
    int y = 1;
    int maxy, maxx;
    getmaxyx(inputWindow, maxy, maxx);
    while (true) {
        ch = getch();
        switch (ch) {
            case ENTER:
                if (input.length() > 0) {
                    // send the message
                    int clientSocket = main::getSocket();
                    send(clientSocket, input.c_str(), input.length(), 0);
                    std::cout << "Sent: " << input << std::endl;
                    // clear the input
                    input = "";
                    x = 1;
                    y = 1;
                    wclear(inputWindow);
                    box(inputWindow, 0, 0);
                    mvwprintw(inputWindow, 0, 5, "Input");
                    wattron(inputWindow, A_BOLD);
                    wrefresh(inputWindow);
                }
                break;
            case BACKSPACE:
                if (x > 1) {
                    x--;
                    mvwprintw(inputWindow, y, x, " ");
                    wmove(inputWindow, y, x);
                    input.pop_back();
                }
                break;
            case ESC:
                activeWindow = 0;
                highlightActiveWindow();
                return;
            default:
                if (x < maxx - 1) {
                    mvwprintw(inputWindow, y, x, "%c", ch);
                    wmove(inputWindow, y, x + 1);
                    x++;
                    input += std::to_string(ch);
                }
                break;
        }
        wrefresh(inputWindow);
    }
}

void GUI::FileAdderToggle() {
    if (!FileAdder) {
        FileAdder = true;
        wclear(FileBrowser);
        box(FileBrowser, 0, 0);
        mvwprintw(FileBrowser, 0, 5, "File Browser");
        wattron(FileBrowser, A_BOLD);
        wrefresh(FileBrowser);
    } else {
        FileAdder = false;
        // remove the box
        wclear(FileBrowser);
        wrefresh(FileBrowser);
    }


}

void GUI::controlHandler() {
    while (true) {
        int ch = getch();
        switch (ch) {
            case TAB:
                activeWindow = (activeWindow + 1) % 3;
                highlightActiveWindow();
                break;
            case SHIFT + TAB:
                activeWindow = (activeWindow - 1) % 3;
                highlightActiveWindow();
                break;
            case CTRL + ALT + ENTER:
                FileAdderToggle();
                break;
            case ESC:
                endwin();
                main::closeClient();
                exit(0);
                break;
            case I:
                activeWindow = 1;
                highlightActiveWindow();
                wrefresh(inputWindow);
                InputMode();
                break;
            default:
                break;
        }
    }
    exit(0);
}
