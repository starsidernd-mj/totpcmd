#include <ncurses.h>
#include <string>
#include <vector>
#include <ctime>
#include <chrono>
#include <thread>
#include <termios.h>
#include <unistd.h>
#include <iostream>

#include "totp.cpp"

// Function declarations
void initialize();
void drawMainMenu();
void cleanup();
void enableNonBlockingInput();
void restoreBlockingInput();

std::string secretKey = "";
std::string totp = "";

int main() {
    initialize(); // Initialize ncurses
    
    // Enable non-blocking input
    enableNonBlockingInput();

    // Main loop
    bool running = true;
    while (running) {
        drawMainMenu(); // Draw the main menu
        //clear();

        // Get user input
        int choice = getch();

        // Process user input
        if(choice != ERR) {
            switch (choice) {
                case '1': {
                    // Handle entering password
                    mvprintw(12, 1, "Enter Password: ");
                    refresh();
                    char password[100]; // Adjust size according to your requirements
                    int i = 0;
                    while (true) {
                        int ch = getch();
                        if (ch == '\n' || ch == '\r') {
                            break; // End of input
                        } else if (ch == '\b' || ch == 127) {
                            // Handle backspace
                            if (i > 0) {
                                --i;
                                mvprintw(12, 16 + i, " "); // Clear the character
                                refresh();
                            }
                        } else {
                            // Handle normal character input
                            if (i < sizeof(password) - 1) {
                                password[i++] = ch;
                                mvprintw(12, 16 + i - 1, "*"); // Display asterisk
                                refresh();
                            }
                        }
                    }
                    password[i] = '\0'; // Null-terminate the password string
                    // Now you have the password in the 'password' variable
                    secretKey.assign(password);
                    break;
                }
                case '2': {
                    // Add your functionality for option 2 here
                    totp = generateTOTP(secretKey);
                    mvprintw(7, 1, "TOTP: %s", totp.c_str());
                    refresh(); 
                    break;
                }
                case 'q':
                case 'Q':
                    running = false; // Quit the program
                    break;
                default:
                    break;
            }
        }
        
        // Get current time
        time_t now = time(0);
        tm *ltm = localtime(&now);

        // Print current time
        mvprintw(9, 1, "Current Time: %02d:%02d:%02d", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);

        refresh(); // Refresh the screen
            
        // Wait for 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    // Restore blocking input before exiting
    restoreBlockingInput();

    cleanup(); // Cleanup ncurses
    return 0;
}

// Function to enable non-blocking input
void enableNonBlockingInput() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

// Function to restore blocking input
void restoreBlockingInput() {
    struct termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    oldt.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void initialize() {
    initscr(); // Initialize ncurses
    cbreak(); // Disable line buffering
    noecho(); // Don't echo user input
    keypad(stdscr, true); // Enable keypad for arrow keys, etc.
}

void drawMainMenu() {
    clear(); // Clear the screen

    // Print the menu options
    mvprintw(1, 1, "Main Menu");
    mvprintw(3, 1, "1. Enter secret key");
    mvprintw(4, 1, "2. Generate TOTP");
    mvprintw(5, 1, "Q. Quit");
    mvprintw(7, 1, "TOTP: %s", totp.c_str());
    
    // Get current time
    time_t now = time(0);
    tm *ltm = localtime(&now);

    // Print current time
    mvprintw(9, 1, "Current Time: %02d:%02d:%02d", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);

    refresh(); // Refresh the screen
}

void cleanup() {
    endwin(); // Cleanup ncurses
}
