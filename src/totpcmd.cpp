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
#include "screenconfig.cpp"
#include "entry.cpp"

// Function declarations
void initialize();
void drawMainMenu();
void cleanup();
void enableNonBlockingInput();
void restoreBlockingInput();
void drawAddMenu();
void loopAddMenu(Entry *entry, bool *running);

std::string secretKey = "";
std::string totp = "";

int main() {
    // System-specific commands to set terminal size and position
    ScreenConfig* sc = new ScreenConfig();
    
    // Get screen size
    int screen_rows, screen_cols;
    sc->get_screen_resolution(&screen_rows, &screen_cols);
    
    // Calculate the rows and columns to set the window size to
    int dst_rows, dst_cols;
    sc->calculate_terminal_size(screen_rows, screen_cols, 8, 16, &dst_rows, &dst_cols);
    
    //std::cout << "Screen res " << dst_rows << ", " << dst_cols << std::endl;
    
    // Resize the terminal
    sc->resize_terminal(dst_rows, 80);
    
    // Get terminal position
    int screen_x, screen_y;
    sc->get_terminal_position(&screen_x, &screen_y);
    //sc.position_terminal();
    
    // Destory ScreenConfig object
    delete sc;
    
    initialize(); // Initialize ncurses
    
    // Enable non-blocking input
    //enableNonBlockingInput();
    nodelay(stdscr, true);

    // Draw the main menu
    drawMainMenu();
    
    // Main loop
    bool running = true;
    // bool to keep menu open
    bool add_menu_running = true;
    
    
    while (running) {
        drawMainMenu(); // Draw the main menu

        // Get user input
        int choice = getch();

        // Process user input
        if(choice != ERR) {
            switch (choice) {
                case '1': {
                    Entry* en = new Entry();
                    add_menu_running = true;
                    
                    nodelay(stdscr, false);
                    
                    while(add_menu_running) {
                        drawAddMenu();
                        loopAddMenu(en, &add_menu_running);
                    }
                    // Handle entering password
                    //mvprintw(12, 1, "Enter Password: ");
                    //refresh();
                    
                    //en->addToken();
                    nodelay(stdscr, true);
                    
                    delete en;
                    break;
                }
                case '2': {
                    // Add your functionality for option 2 here
                    //replace time step
                    uint64_t timeStep = 30;

                    //get current time in seconds
                    uint64_t currentTime = std::time(nullptr);

                    //calculate counter based on current time and time step
                    uint64_t counter = currentTime / timeStep;
                    
                    TOTP totpGen;
                    totp = totpGen.generateTOTP(secretKey, counter);
                    mvprintw(7, 1, "TOTP: %s", totp.c_str());
                    refresh(); 
                    break;
                }
                case 'q':
                case 'Q':
                    running = false; // Quit the program
                    break;
            }
        }
        
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
    
    //usleep(100000);
}

// entry items
std::string entry_name = "";
std::string entry_token = "";
char entry_token_hidden[20];

void drawAddMenu() {
    clear();
    
    // Print header
    mvprintw(1, 1, "Add item");
    mvprintw(3, 1, "1. Name: ");
    mvprintw(4, 1, "2. Issuer: ");
    mvprintw(5, 1, "3. Token: ");
    mvprintw(6, 1, "4. Create");
    mvprintw(7, 1, "Q. Cancel");
    
    mvprintw(3, 16, entry_name.c_str());
    mvprintw(5, 16, entry_token_hidden);
    
    refresh();
}

void loopAddMenu(Entry *entry, bool *running) {
    // Get user input
    int choice = getch();

    // Process user input
    if(choice != ERR) {
        switch (choice) {
            case '1': {
                entry_name = entry->addName();
                break;
            }
            case '2': {
                
                break;
            }
            case '3': {
                entry_token = entry->addToken();
                for(int i = 0; i < entry_token.length(); i++) {
                    entry_token_hidden[i] = '*';
                }
                break;
            }
            case '4': {
                break;
            }
            case 'q':
            case 'Q':
                *running = false; 
                break;
            default: break;
        }
    }
}

void cleanup() {
    endwin(); // Cleanup ncurses
}
